<?php
/**
 * @file
 * Classe of Univers element
 * 
 * @author  Barthélemi Laurent <lba@audaxis.com>
 * @package DPI
 * @version  1.0
 */

module_load_include('inc', 'dpiunivers', 'dpiunivers.api');

class DpiUnivers {
  
  private $id;
  private $label;
  private $theme;
  private $menu;  
  
  function __construct($id = NULL) {
    if(isset($id)){
      $sav = dpiunivers_api_get_one_univers($id);
      if(isset($sav)){
        $this->id = $sav->id;
        $this->label = $sav->label;
        $this->theme = $sav->theme;
        $this->menu = $sav->menu;
      }else{
        return NULL;
      }
    }else{
      $this->id=NULL;
      $this->label="";
      $this->theme="";
      $this->menu="";
    }
  }
  
  function getId() {
    return $this->id;
  }

  function getLabel() {
    return $this->label;
  }

  function getTheme() {
    return $this->theme;
  }

  function getMenu() {
    return $this->menu;
  }

  function setId($id) {
    $this->id = $id;
  }

  function setLabel($label) {
    $this->label = $label;
  }

  function setTheme($theme) {
    $this->theme = $theme;
  }

  function setMenu($menu) {
    $this->menu = $menu;
  }

  public function __toString() {
    return 'Univers = [ ID => '.$this->id.' , Label => '.$this->label.' , Theme => '.$this->theme.' , Menu => '.$this->menu.' ]';
  }
  
}