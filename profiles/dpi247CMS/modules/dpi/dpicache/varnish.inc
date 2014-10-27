<?php

define('VARNISH_NO_CLEAR', 0);
define('VARNISH_DEFAULT_CLEAR', 1);
define('VARNISH_SELECTIVE_CLEAR', 2); // Requires Expire.module to be enabled.
// Timeout in milliseconds.
define('VARNISH_DEFAULT_TIMEOUT', 100);
define('VARNISH_SERVER_STATUS_DOWN', 0);
define('VARNISH_SERVER_STATUS_UP', 1);
define('VARNISH_BANTYPE_NORMAL', 0);
define('VARNISH_BANTYPE_BANLURKER', 1);
define('VARNISH_DEFAULT_BANTYPE', VARNISH_BANTYPE_NORMAL);



/**
 * Helper function to quickly flush all caches for the current site.
 */
function varnish_purge_all_pages() {
  $path = base_path();
  $host = _varnish_get_host();
  varnish_purge($host, $path);
}

/**
 * return host url
 */
function _varnish_get_host() {
  global $base_url;
  $parts = parse_url($base_url);
  return $parts['host'];
}

/**
 * Helper function to purge items for a host that matches the provided pattern.
 * @param string $host the host to purge.
 * @param string $pattern the pattern to look for and purge.
 */
function varnish_purge($host, $pattern,$force_direct_process=FALSE) {
  //Check purge method: direct or accumulator.
  _varnish_purge($host, $pattern);
}

/**
 * Helper function to purge items for a host that matches the provided pattern.
 * @param string $host the host to purge.
 * @param string $pattern the pattern to look for and purge.
 */
function _varnish_purge($host, $pattern) {
  
  // Get the current varnish version, if we are using Varnish 3.x, then we can
  // need to use ban instead of purge.
  $version = floatval(variable_get('varnish_version', 2.1));
  $command = $version >= 3 ? "ban" : "purge";
  $bantype = variable_get('varnish_bantype', VARNISH_DEFAULT_BANTYPE);
  switch ($bantype) {
    case VARNISH_BANTYPE_NORMAL:
      _varnish_terminal_run(array("$command req.http.host ~ $host && req.url ~ \"$pattern\""));
      break;
    case VARNISH_BANTYPE_BANLURKER:
      _varnish_terminal_run(array("$command obj.http.x-host ~ $host && obj.http.x-url  ~ \"$pattern\""));
      break;
    default:
      // We really should NEVER get here. Log WATCHDOG_ERROR. I can only see this happening if a user switches between different versions of the module where we remove a ban type.
      watchdog('varnish', 'Varnish ban type is out of range.', array(), WATCHDOG_ERROR);
  }
}


/**
 * Get the status (up/down) of each of the varnish servers.
 *
 * @return An array of server statuses, keyed by varnish terminal addresses.
 * The status will be a numeric constant, either:
 * - VARNISH_SERVER_STATUS_UP
 * - VARNISH_SERVER_STATUS_DOWN
 */
function varnish_get_status() {
  // use a static-cache so this can be called repeatedly without incurring
  // socket-connects for each call.
  static $results = NULL;
  if (is_null($results)) {
    $results = array();
    $status = _varnish_terminal_run(array('status'));
    $terminals = explode(' ', variable_get('varnish_control_terminal', '127.0.0.1:6082'));
    foreach ($terminals as $terminal) {
      $stat = array_shift($status);
      $results[$terminal] = ($stat['status']['code'] == 200) ? VARNISH_SERVER_STATUS_UP : VARNISH_SERVER_STATUS_DOWN;
    }
  }
  return $results;
}

/**
 * Helper function that sends commands to Varnish.
 * Utilizes sockets to talk to varnish terminal.
 */
function _varnish_terminal_run($commands) {
  if (!extension_loaded('sockets')) {
    // Prevent fatal errors if people don't have requirements.
    return FALSE;
  }
  // Convert single commands to an array so we can handle everything in the same way.
  if (!is_array($commands)) {
    $commands = array($commands);
  }
  $ret = array();
  $terminals = explode(' ', variable_get('varnish_control_terminal', '127.0.0.1:6082'));
  // The variable varnish_socket_timeout defines the timeout in milliseconds.
  $timeout = variable_get('varnish_socket_timeout', VARNISH_DEFAULT_TIMEOUT);
  $seconds = (int)($timeout / 1000);
  $microseconds = (int)($timeout % 1000 * 1000);
  foreach ($terminals as $terminal) {
    list($server, $port) = explode(':', $terminal);
    $client = socket_create(AF_INET, SOCK_STREAM, getprotobyname('tcp'));
    socket_set_option($client, SOL_SOCKET, SO_SNDTIMEO, array('sec' => $seconds, 'usec' => $microseconds));
    socket_set_option($client, SOL_SOCKET, SO_RCVTIMEO, array('sec' => $seconds, 'usec' => $microseconds));
    if (@!socket_connect($client, $server, $port)) {
      watchdog('varnish', 'Unable to connect to server socket @server:@port: %error', array(
      '@server' => $server,
      '@port' => $port,
      '%error' => socket_strerror(socket_last_error($client))
      ), WATCHDOG_ERROR);
      $ret[$terminal] = FALSE;
      // If a varnish server is unavailable, move on to the next in the list.
      continue;
    }
    // If there is a CLI banner message (varnish >= 2.1.x), try to read it and move on.
    if(floatval(variable_get('varnish_version', 2.1)) > 2.0) {
      $status = _varnish_read_socket($client);
      // Do we need to authenticate?
      if ($status['code'] == 107) { // Require authentication
        $secret = variable_get('varnish_control_key', '');
        $challenge = substr($status['msg'], 0, 32);
        $pack = $challenge . "\x0A" . $secret . "\x0A" . $challenge . "\x0A";
        $key = hash('sha256', $pack);
        socket_write($client, "auth $key\n");
        $status = _varnish_read_socket($client);
        if ($status['code'] != 200) {
          watchdog('varnish', 'Authentication to server failed!', array(), WATCHDOG_ERROR);
        }
      }
    }
    foreach ($commands as $command) {
      if ($status = _varnish_execute_command($client, $command)) {
        $ret[$terminal][$command] = $status;
      }
    }
  }
  return $ret;
}

function _varnish_execute_command($client, $command) {
  // Send command and get response.
  $result = socket_write($client, "$command\n");
  $status = _varnish_read_socket($client);
  if ($status['code'] != 200) {
    watchdog('varnish', 'Recieved status code @code running %command. Full response text: @error', array('@code' => $status['code'], '%command' => $command, '@error' => $status['msg']), WATCHDOG_ERROR);
    return FALSE;
  }
  else {
    // successful connection
    return $status;
  }
}

/**
 * Low-level socket read function.
 *
 * @params
 *   $client an initialized socket client
 *
 *   $retty how many times to retry on "temporarily unavalble" errors
 */
function _varnish_read_socket($client, $retry = 2) {
  // Status and length info is always 13 characters.
  $header = socket_read($client, 13, PHP_BINARY_READ);
  if ($header == FALSE) {
    $error = socket_last_error();
    // 35 = socket-unavailable, so it might be blocked from our write.
    // This is an acceptable place to retry.
    if ($error == 35 && $retry > 0) {
      return _varnish_read_socket($client, $retry-1);
    }
    else {
      watchdog('varnish', 'Socket error: @error', array('@error' => socket_strerror($error)), WATCHDOG_ERROR);
      return array(
        'code' => $error,
        'msg' => socket_strerror($error),
      );
    }
  }
  $msg_len = (int)substr($header, 4, 6) + 1;
  $status = array(
    'code' => substr($header, 0, 3),
    'msg' => socket_read($client, $msg_len, PHP_BINARY_READ)
  );
  return $status;
}
