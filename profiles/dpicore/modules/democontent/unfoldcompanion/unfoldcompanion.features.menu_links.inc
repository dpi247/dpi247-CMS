<?php
/**
 * @file
 * unfoldcompanion.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function unfoldcompanion_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_economy:section/15
  $menu_links['main-menu_economy:section/15'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/15',
    'router_path' => 'section/%',
    'link_title' => 'Economy',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_economy:section/15',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: main-menu_news:section/12
  $menu_links['main-menu_news:section/12'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/12',
    'router_path' => 'section/%',
    'link_title' => 'News',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_news:section/12',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: main-menu_politics:section/14
  $menu_links['main-menu_politics:section/14'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/14',
    'router_path' => 'section/%',
    'link_title' => 'Politics',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_politics:section/14',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: main-menu_sports:section/13
  $menu_links['main-menu_sports:section/13'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/13',
    'router_path' => 'section/%',
    'link_title' => 'Sports',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_sports:section/13',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Economy');
  t('News');
  t('Politics');
  t('Sports');


  return $menu_links;
}
