<?php 

class ImporterStreamWrapper extends DrupalPublicStreamWrapper {
  public function getDirectoryPath() {
    return variable_get('importerstreamwrapper', conf_path() . '/importer');
  }
}
