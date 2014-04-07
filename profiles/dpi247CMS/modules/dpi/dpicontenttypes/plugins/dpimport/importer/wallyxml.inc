<?php 

$plugin = array(
  'label' => t('Wally XML'),
  'type'  => 'wallyxml',
  'versions' => array(
    '1' => array('class' => 'WallyXmlV1',),
  ),
  'dispatchers' => array(
    'FolderDispatcher',
    'UrlDispatcher',
    'PostDispatcher',
  ),
);