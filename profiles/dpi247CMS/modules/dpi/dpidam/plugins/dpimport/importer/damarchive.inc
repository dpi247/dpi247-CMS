<?php 

$plugin = array(
  'label' => t('DAM Archive'),
  'type'  => 'damarchive',
  'versions' => array(
    '1' => array(
      'class' => 'DamArchiveV1',
      'include' => array(
        'module' => 'dpidam',
        'path' => 'plugins/dpimport/importer/damarchivev1.class',
      ),
    ),
  ),
  'dispatchers' => array(
    'FolderDispatcher',
  ),
);
