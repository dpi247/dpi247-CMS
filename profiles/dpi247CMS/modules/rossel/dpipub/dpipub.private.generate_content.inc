<?php

/**global
 * Get content for the pub block.
*
* @param string $delta
*   The delta of the concerned block.
*
* @return
*   HTML content.
*
* @see http://api.drupal.org/api/drupal/developer--hooks--core.php/function/hook_block/6
*   Hook_block
*/
function _pub_get_pub_content($delta, $cache_url) {
  $current_page = menu_get_item($_GET['q']);
  if (!$current_page) {
    return '';
  }

  $cached_pub = cache_get('pub_'.$delta.'_temp-'.$cache_url);
  $block_settings = pub_load_block($delta);
  
  if($block_settings=="dfp"){
    global $dfp_viewed_link_blocks;
    if(!isset($dfp_viewed_link_blocks) || $dfp_viewed_link_blocks==""){
      $webserv_req = _pub_getwebservreq($current_page['href']);
      $website = variable_get('pub_dfp_website', '');
      $expl_website = explode('_', $website);
      $dfp_viewed_link_blocks = "?scriptType=DFP&site=".rawurlencode($expl_website[0])."&context=".'/'.rawurlencode($webserv_req[0])."&pageType=".rawurlencode($webserv_req[1])."&domain=".rawurlencode($_SERVER['HTTP_HOST']);
    }
    $dfp_viewed_link_blocks .= "&positions=".$delta;
  }
  
  if (!is_object($cached_pub) || !isset($cached_pub) || empty($cached_pub) || $cached_pub->expire < time()) {
    $webserv_req = _pub_getwebservreq($current_page['href']);
    switch ($block_settings) {
      case 'purl':
        $pub_content = _pub_generate_purl_pub_content($block_settings, $delta, $webserv_req, '_purl', $cache_url);
        if ($pub_content['meta'] && !empty($pub_content['meta']['content'])) {
          pub_add_meta($pub_content['meta']['attributes'], $pub_content['meta']['content']);
        }
        break;
      case 'openx' || 'dfp':
        $pub_content = _pub_generate_default_pub_content($block_settings, $delta, $webserv_req, '_'.$block_settings, $cache_url);
        break;
      default:
        $pub_content = _pub_generate_default_pub_content($block_settings, $delta, $webserv_req, '', $cache_url);
    }
    /* Create log message */
    if(function_exists('dpi_log'))
      dpi_log('Dpipub', "end_message", 'Content is created by webservice');
    
    /* Set pub content to cache */
    cache_set('pub_'.$delta.'_temp-'.$cache_url, $pub_content, 'cache', time() + 3600*variable_get('pub_block_'.$delta.'_refresh_time', 24));
    cache_set('pub_'.$delta.'_perm-'.$cache_url, $pub_content);
  } else {
    /* Create message log */
    if(function_exists('dpi_log'))
      dpi_log('Dpipub', "end_message", 'End - pub is created from cache');    
    
    /* GET pub content to cache */
    $pub_content = $cached_pub->data;
    if (isset($pub_content['meta']) && !empty($pub_content['meta']['content'])) {
      pub_add_meta($pub_content['meta']['attributes'], $pub_content['meta']['content']);
    }
    /* GET header content */
    if($block_settings == 'dfp'){
      global $var;
      if($var!=true){
        $val = cache_get("dfp_header_".$current_page['href']);        
        drupal_add_js($val->data,'inline');
        $var=true;
      }
    }
  }
  return theme('pub_'.$block_settings.'_content', array( 'html_code' => $pub_content,'delta' => $delta));
}

/**
 * This function return default pub content
 * @param NOTUSE $block_settings
 * @param String $delta
 * @param String $webserv_req
 * @param String $builder
 * @param String $cache_url
 * @return Ambigous <HTML, string>
 */
function _pub_generate_default_pub_content($block_settings, $delta, $webserv_req, $builder, $cache_url) {
  $script_type = variable_get('pub'.$builder.'_provider', 'AdTech');
  $website = variable_get('pub'.$builder.'_website', '');
  $expl_website = explode('_', $website);
  $edition = isset($webserv_req[2]) ? $webserv_req[2] : NULL;
  $domain = $_SERVER['HTTP_HOST'];
  $params = array(
    'scriptType' => $script_type,
    'site' => $expl_website[0],
    'pageType' => $webserv_req[1],
    'context' => '/'.$webserv_req[0],
    'position' => $delta,
    'domain' => $domain,
  );
  if ($edition) {
    $params['edition'] = $edition;
  }
  $webserv_adress = variable_get('pub'.$builder.'_webserv', 'http://wspub1.rossel.be:8080/Wally-Tags/rest/wally/script/detail');

  if ($content_from_webservice = _pub_get_content_from_webservice($webserv_adress, $params, $builder)) {
    $pub_content = $content_from_webservice;
  } else {
    $cached_pub = cache_get('pub_'.$delta.'_perm-'.$cache_url);
    $pub_content = $cached_pub->data ? $cached_pub->data : '';
  }
  return $pub_content;
}

/**
 * Special function for purl pub
 * @param String $block_settings
 * @param String $delta
 * @param String $webserv_req
 * @param String $builder
 * @param String $cache_url
 * @return Ambigous <multitype:The , string>
 */
function _pub_generate_purl_pub_content($block_settings, $delta, $webserv_req, $builder, $cache_url) {
  $script_type = variable_get('pub'.$builder.'_provider', 'AdTech');
  $website = variable_get('pub'.$builder.'_website', '');
  $expl_website = explode('_', $website);
  $edition = isset($webserv_req[2]) ? $webserv_req[2] : NULL;
  $domain = $_SERVER['HTTP_HOST'];
  $params = array(
    'scriptType' => $script_type,
    'site' => $expl_website[0],
    'pageType' => $webserv_req[1],
    'context' => '/'.$webserv_req[0],
    'position' => $delta,
    'domain' => $domain,
  );
  if ($edition) {
    $params['edition'] = $edition;
  }
  $webserv_adress = variable_get('pub'.$builder.'_webserv', 'http://wspub1.rossel.be:8080/Wally-Tags/rest/wally/script/detail');
  $header_adress = variable_get('pub'.$builder.'_header', 'http://wspub1.rossel.be:8080/Wally-Tags/rest/wally/script/header');

  $content_from_webservice = _pub_get_content_from_webservice($webserv_adress, $params, '');
  $header_from_webservice = _pub_get_content_from_webservice($header_adress, $params, '');

  if ($content_from_webservice && !empty($content_from_webservice) && $header_from_webservice && !empty($header_from_webservice)) {
    $pub_content['content'] = $content_from_webservice;
    $pub_content['meta']['content'] = $header_from_webservice;
    $pub_content['meta']['attributes'] = array(
      'name' => variable_get('pub'.$builder.'_meta_name', 'adpurl-keywords'),
    );
  } else {
    $cached_pub = cache_get('pub_'.$delta.'_perm-'.$cache_url);
    $pub_content = $cached_pub->data ? $cached_pub->data : '';
  }

  return $pub_content;
}

/**
 * Get content for the pub header block.
 *
 * @return
 *   HTML content.
 *
 * @see http://api.drupal.org/api/drupal/developer--hooks--core.php/function/hook_block/6
 *   Hook_block
 */
function _pub_getheadercontent($cache_url) {
  $current_page = menu_get_item($_GET['q']);
  if (!$current_page) {
    return '';
  }
  $orig_url = $current_page['href'];
  $cached_pub = cache_get('header_temp-'.$cache_url);
  $cache_time = variable_get('header_refresh_time', 24);

  if (!is_object($cached_pub) || !isset($cached_pub) || empty($cached_pub) || $cached_pub->expire < time()) {
    $script_type = variable_get('pub_provider', 'AdTech');
    $website = variable_get('pub_website', '');
    $expl_website = explode('_', $website);
    $webserv_req = _pub_getwebservreq($orig_url);
    $edition = isset($webserv_req[2]) ? $webserv_req[2] : NULL;
    $domain = $_SERVER['HTTP_HOST'];
    $params = array(
      'scriptType' => $script_type,
      'site' => $expl_website[0],
      'pageType' => $webserv_req[1],
      'context' => '/'.$webserv_req[0],
      'position' => (isset($delta))? $delta : '',
      'domain' => $domain,
    );
    if ($edition) {
      $params['edition'] = $edition;
    }
    $header_content = '';
    $header_adress = variable_get('pub_header', '');
    if($header_adress!=''){
      if ($content_from_webservice = _pub_get_content_from_webservice($header_adress, $params, '')) {
        $header_content = $content_from_webservice;
      } else {
        $cached_pub = cache_get('header_perm-'.$cache_url);
        $header_content = $cached_pub->data ? $cached_pub->data : '';
      }
    }
    cache_set('header_temp-'.$cache_url, $header_content, 'cache', time() + 3600*$cache_time);
    cache_set('header_perm-'.$cache_url, $header_content);
  } else {
    $header_content = $cached_pub->data;
  }
  if($header_content==''){ drupal_set_message('Configuration is not avalaible for header pub block !!!','error');}
  return theme('pub_header', array( 'header_code' => $header_content));
}

/**
 * Get content a generic block.
 *
 * @return
 *   HTML content.
 *
 * @see http://api.drupal.org/api/drupal/developer--hooks--core.php/function/hook_block/6
 *   Hook_block
 */
function _pub_get_block_content($cache_url, $type, $script_type, $delta = '') {
  $current_page = menu_get_item($_GET['q']);
  if (!$current_page) {
    return '';
  }
  $orig_url = $current_page['href'];
  $cached_pub = cache_get($type.'_block_temp-'.$cache_url, 'cache');
  $cache_time = variable_get($type.'_block_refresh_time', 24);

  if (!is_object($cached_pub) || !isset($cached_pub) || empty($cached_pub) || $cached_pub->expire < time()) {
    $website = variable_get('pub_website', '');
    $expl_website = explode('_', $website);
    $webserv_req = _pub_getwebservreq($orig_url);
    $domain = $_SERVER['HTTP_HOST'];
    $params = array(
      'scriptType' => $script_type,
      'site' => $expl_website[0],
      'pageType' => $webserv_req[1],
      'context' => '/'.$webserv_req[0],
      'position' => $delta,
      'domain' => $domain,
    );

    $webserv_adress = variable_get('pub_webserv', 'http://wspub1.rossel.be:8080/Wally-Tags/rest/wally/script/detail');
    if ($content_from_webservice = _pub_get_content_from_webservice($webserv_adress, $params, '')) {
      $metriweb_content = $content_from_webservice;
    } else {
      $cached_pub = cache_get($type.'_block_perm-'.$cache_url, 'cache');
      $metriweb_content = $cached_pub->data ? $cached_pub->data : '';
    }

    cache_set($type.'_block_temp-'.$cache_url, $metriweb_content, 'cache', time() + 3600*$cache_time);
    cache_set($type.'_block_perm-'.$cache_url, $metriweb_content, 'cache');
  } else {
    $metriweb_content = $cached_pub->data;
  }
  return theme($type.'_content', array( 'html_code' => $metriweb_content));
}

/**
 * Get content from a webservice.
 *
 * @return
 *   HTML content.
 */
function _pub_get_content_from_webservice($url, $params, $builder) {
  drupal_alter('pub_params', $params , $builder);
  foreach ($params as $key => $value) {
    $params[$key] = $key.'='.rawurlencode($value);
  }
  $req_params = '?'.implode('&', $params);
  $options = array(
    'method' => 'GET',
    'data' => NULL,
    'timeout' => variable_get('dpicommons_http_request_timeout', 3),
    'retry' => variable_get('dpicommons_http_request_retry', 1),
  );
  $content = drupal_http_request($url.$req_params, $options);
  $context = "context=no context";
  if(isset($params["context"]))
    $context = $params["context"];
  if(function_exists('dpi_log'))
    dpi_log('Dpipub', "threading", $builder.' - ['.$context.'] Web service content from : '.$url.$req_params);  
  return ($content->code == '200') ? $content->data : FALSE;
}

/**
 * Return the request sent to the webservice. The request is based on the URL.
 *
 * @param string $orig_url
 *   The page URL, given by $_GET['q'].
 *
 * @return
 *   The request as a string.
 */
function _pub_getwebservreq($orig_url) {
  $expl_url = explode('/', $orig_url);
  $webserv_req = array();
  $home_page = variable_get('site_frontpage', '');
  if ($home_page == $orig_url) {
    $webserv_req[] = 'home';
    $webserv_req[] = 'home';
  } else {
    switch ($expl_url[0]) {
      case 'node':
        if (isset($expl_url[1]) && $expl_url[1]!=null) {
          $node = node_load($expl_url[1]);
          if (module_exists('cckdestinations') && isset($node->field_destinations) && !empty($node->field_destinations)) {
            $node_dest = $node->field_destinations;
            $tax_hierar = pub_taxonomy_get_path_by_tid_or_term($node_dest[0]['tid'], '', TRUE);
            if ($tax_hierar == '' || empty($tax_hierar)) {
              $webserv_req[] = $orig_url;
            } else {
              $webserv_req[] = $tax_hierar.'/'.$node->nid;
            }
          } else {
            $webserv_req[] = $orig_url;
          }
        } else {
          $webserv_req[] = $orig_url;
        }
        $webserv_req[] = 'node';
        if (isset($node->field_editions) && isset($node->field_editions[0]) && is_numeric($node->field_editions[0]['value'])) {
          $edition_term = taxonomy_get_term($node->field_editions[0]['value']);
          $webserv_req[] = $edition_term->name;
        }
        break;
      case 'taxonomy':
        if ($expl_url[2]) {
          $tids = $expl_url[2];
          $expl_tids = preg_split('(\+|\,|\ )', $tids);
          $tax_hierar = pub_taxonomy_get_path_by_tid_or_term($expl_tids[0], '', TRUE);
          $webserv_req[] = $tax_hierar;
        } else {
          $webserv_req[] = $orig_url;
        }
        $webserv_req[] = 'page';
        break;
      default:
        $primary_links_tree = menu_tree_page_data('primary-links');
        $webserv_req = _pub_get_path_to_item($primary_links_tree, $orig_url, TRUE);
        if (!is_array($webserv_req)) {
          $webserv_req = array();
          $webserv_req[] = $orig_url;
          $webserv_req[] = 'page';
        }
    }
  }

  return $webserv_req;
}

/**
 * Recursively search the menu tree for an URL. When found return the
 * taxonomy tree related to his direct parent plus himself.
 *
 * @param $menu_step
 *   The menu level (with all the menu elements of this level)
 *   where the search curently is.
 * @param $link_path
 *   The searched URL.
 */
function _pub_get_path_to_item($menu_step, $link_path, $first_level = FALSE) {
  $temp_name = FALSE;
  foreach ($menu_step as $elem_name => $menu_elem) {
    if ($menu_elem['link']['link_path'] == $link_path) {
      if ($first_level) {
        $result = _pub_getwebservreq($menu_elem['link']['link_path']);
        $result[0] = ($result[0] == '') ? $link_path : $result[0].'/'.$link_path;
        return $result;
      } else {
        return 'found';
      }
    } elseif ($menu_elem['below']) {
      $next_lev = _pub_get_path_to_item($menu_elem['below'], $link_path);
      if ($next_lev == 'found' && $menu_elem['link']['module'] == 'taxonomy_menu') {
        $result = _pub_getwebservreq($menu_elem['link']['link_path']);
        $result[0] = ($result[0] == '') ? $link_path : $result[0].'/'.$link_path;
        return $result;
      } elseif (is_array($next_lev)) {
        return $next_lev;
      }
    }
  }
  return 'not found';
}

