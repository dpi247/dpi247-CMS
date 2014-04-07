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

  // Exported menu link: main-menu_economy:section/4
  $menu_links['main-menu_economy:section/4'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/4',
    'router_path' => 'section/%',
    'link_title' => 'Economy',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_economy:section/4',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -47,
    'customized' => 1,
  );
  // Exported menu link: main-menu_news:section/1
  $menu_links['main-menu_news:section/1'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/1',
    'router_path' => 'section/%',
    'link_title' => 'News',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_news:section/1',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
  );
  // Exported menu link: main-menu_politics:section/2
  $menu_links['main-menu_politics:section/2'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/2',
    'router_path' => 'section/%',
    'link_title' => 'Politics',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_politics:section/2',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -49,
    'customized' => 1,
  );
  // Exported menu link: main-menu_sports:section/3
  $menu_links['main-menu_sports:section/3'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'section/3',
    'router_path' => 'section/%',
    'link_title' => 'Sports',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_sports:section/3',
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
