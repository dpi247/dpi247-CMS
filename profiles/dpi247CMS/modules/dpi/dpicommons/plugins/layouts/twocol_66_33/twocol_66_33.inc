<?php
// Plugin definition
$plugin = array(
  'title' => t('Two column 66/33'),
  'category' => t('Columns: 2'),
  'icon' => 'twocol_66_33.png',
  'theme' => 'dpicommons_twocol_66_33',
  'css' => 'twocol_66_33.css',
  'regions' => array(
    'left' => t('Left side'),
    'right' => t('Right side'),
    
    'tag1' => t('Tag1'),
    'tag2' => t('tag2'),
    'tag3' => t('Tag3'),
    
    
    'bottom' => t('Bottom'),
   ),
);
