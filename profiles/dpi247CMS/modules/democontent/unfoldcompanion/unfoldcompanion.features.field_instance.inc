<?php

/**
 * @file
 * unfoldcompanion.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function unfoldcompanion_field_default_field_instances() {
  $field_instances = array ();
  
  // Exported field_instance: 'comment-comment_node_package-comment_body'
  $field_instances ['comment-comment_node_package-comment_body'] = array (
    'bundle' => 'comment_node_package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'comment',
    'field_name' => 'comment_body',
    'label' => 'Comment',
    'required' => TRUE,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'text',
      'settings' => array (
        'rows' => 5 
      ),
      'type' => 'text_textarea',
      'weight' => 0 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_authors'
  $field_instances ['node-package-field_authors'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 18 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 4 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 8 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 7 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_authors',
    'label' => 'Authors',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 25 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_autopublish'
  $field_instances ['node-package-field_autopublish'] = array (
    'bundle' => 'package',
    'default_value' => array (
      0 => array (
        'value' => 0 
      ) 
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_autopublish',
    'label' => 'Use embargo and Publish Date',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (
        'display_label' => 1 
      ),
      'type' => 'options_onoff',
      'weight' => 11 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_autopublishdate'
  $field_instances ['node-package-field_autopublishdate'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_autopublishdate',
    'label' => 'Auto publish date',
    'required' => 0,
    'settings' => array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'date',
      'settings' => array (
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array (),
        'year_range' => '-3:+3' 
      ),
      'type' => 'date_select',
      'weight' => 12 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_byline'
  $field_instances ['node-package-field_byline'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_byline',
    'label' => 'Byline',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 2 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_concepts'
  $field_instances ['node-package-field_concepts'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 12 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_concepts',
    'label' => 'Concepts',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 19 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_copyright'
  $field_instances ['node-package-field_copyright'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_copyright',
    'label' => 'Copyright',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 10 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_destinations'
  $field_instances ['node-package-field_destinations'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_destinations',
    'label' => 'Destinations',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'dpidestinations',
      'settings' => array (),
      'type' => 'destination_default_widget',
      'weight' => 28 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_displaytitle'
  $field_instances ['node-package-field_displaytitle'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h1' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 2 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'big-title',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 4 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 1 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h1' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_displaytitle',
    'label' => 'Display Title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 1 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_editorialpublicationdate'
  $field_instances ['node-package-field_editorialpublicationdate'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 10 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 3 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_editorialpublicationdate',
    'label' => 'Editorial Publication date',
    'required' => 0,
    'settings' => array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'date',
      'settings' => array (
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array (),
        'year_range' => '-3:+3' 
      ),
      'type' => 'date_select',
      'weight' => 14 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_editorialupdatedate'
  $field_instances ['node-package-field_editorialupdatedate'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 4 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 4 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_editorialupdatedate',
    'label' => 'Editorial Update Date',
    'required' => 0,
    'settings' => array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'date',
      'settings' => array (
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array (),
        'year_range' => '-3:+3' 
      ),
      'type' => 'date_select',
      'weight' => 13 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_embededobjects'
  $field_instances ['node-package-field_embededobjects'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => TRUE,
          'context' => 'dpi_mediabox',
          'type_number' => 'all',
        ),
        'type' => 'media_box',
        'weight' => 17 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => FALSE,
          'context' => 'dpi_simpleimg',
        ),
        'type' => 'first_element',
        'weight' => 6 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => TRUE,
          'context' => 'dpi_simpleimg',
        ),
        'type' => 'first_element',
        'weight' => 2 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => FALSE,
          'context' => 'dpi_simpleimg',
        ),
        'type' => 'first_element',
        'weight' => 6 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_embededobjects',
    'label' => 'EmbededObjects',
    'required' => 0,
    'settings' => array (
      'field_to_analyse' => 'field_textbody',
      'referencable_types' => array (
        'image' => 'image',
        'video' => 'video',
        'atom_package' => 'atom_package' 
      ),
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'dpicontenttypes',
      'settings' => array (),
      'type' => 'dpiatom_reference_textfield',
      'weight' => 6 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_entities'
  $field_instances ['node-package-field_entities'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 14 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_entities',
    'label' => 'Entities',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 21 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_externalurl'
  $field_instances ['node-package-field_externalurl'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_externalurl',
    'label' => 'External URL',
    'required' => 0,
    'settings' => array (
      'attributes' => array (
        'class' => '',
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => '_blank',
        'title' => '' 
      ),
      'display' => array (
        'url_cutoff' => 80 
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'none',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'link',
      'settings' => array (),
      'type' => 'link_field',
      'weight' => 16 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_freetags'
  $field_instances ['node-package-field_freetags'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_freetags',
    'label' => 'Free Tags',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 22 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_iptc'
  $field_instances ['node-package-field_iptc'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 11 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 7 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 7 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 5 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_iptc',
    'label' => 'IPTC',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 18 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_linkslists'
  $field_instances ['node-package-field_linkslists'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => FALSE,
          'type_links_list' => TRUE,
          'type_video' => FALSE,
          'context' => 'full',          
        ),
        'type' => 'first_element',
        'weight' => 9 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => FALSE,
          'type_links_list' => TRUE,
          'type_video' => FALSE,
          'context' => 'full',          
        ),
        'type' => 'first_element',
        'weight' => 9 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_linkslists',
    'label' => 'Links lists',
    'required' => 0,
    'settings' => array (
      'field_to_analyse' => 'field_textbody',
      'referencable_types' => array (
        'image' => 0,
        'links_list' => 'links_list',
        'video' => 0 
      ),
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'dpicontenttypes',
      'settings' => array (),
      'type' => 'dpiatom_reference_textfield',
      'weight' => 7 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_locations'
  $field_instances ['node-package-field_locations'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 13 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_locations',
    'label' => 'Locations',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 20 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_packagelayout'
  $field_instances ['node-package-field_packagelayout'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 7 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_packagelayout',
    'label' => 'Package Layout',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 17 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_packagetype'
  $field_instances ['node-package-field_packagetype'] = array (
    'bundle' => 'package',
    'default_value' => array (
      array (
        'tid' => 6
      ) 
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 8 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_packagetype',
    'label' => 'Package Type',
    'required' => 1,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (),
      'type' => 'options_select',
      'weight' => 25 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_persons'
  $field_instances ['node-package-field_persons'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 15 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_persons',
    'label' => 'Persons',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 24 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_restrictedaccess'
  $field_instances ['node-package-field_restrictedaccess'] = array (
    'bundle' => 'package',
    'default_value' => array (
      0 => array (
        'value' => 0 
      ) 
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_restrictedaccess',
    'label' => 'Restricted Access',
    'required' => 1,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (),
      'type' => 'options_select',
      'weight' => 27 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textbarette'
  $field_instances ['node-package-field_textbarette'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 3 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 3 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 8 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textbarette',
    'label' => 'Barette',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 4 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textbody'
  $field_instances ['node-package-field_textbody'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 16 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (
          'trim_length' => 400 
        ),
        'type' => 'text_trimmed',
        'weight' => 11 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (
          'trim_length' => 300 
        ),
        'type' => 'text_trimmed',
        'weight' => 9 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (
          'trim_length' => 150 
        ),
        'type' => 'text_trimmed',
        'weight' => 3 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textbody',
    'label' => 'Body',
    'required' => 0,
    'settings' => array (
      'context' => 'full',
      'display_summary' => 0,
      'dnd_enabled' => 1,
      'mee_enabled' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 20,
        'summary_rows' => 5 
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 5 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textchapo'
  $field_instances ['node-package-field_textchapo'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'heading',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 7 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'description',
          'tag' => 'div' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 8 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'heading',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 7 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textchapo',
    'label' => 'Chapo',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 5 
      ),
      'type' => 'text_textarea',
      'weight' => 3 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textforetitle'
  $field_instances ['node-package-field_textforetitle'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 1 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textforetitle',
    'label' => 'Fore title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 3 
      ),
      'type' => 'text_textarea',
      'weight' => 8 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textsubtitle'
  $field_instances ['node-package-field_textsubtitle'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 3 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 3 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textsubtitle',
    'label' => 'Sub Title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 3 
      ),
      'type' => 'text_textarea',
      'weight' => 9 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_authors'
  $field_instances ['node-package_gallery-field_authors'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 18 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 4 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 8 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 7 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_authors',
    'label' => 'Authors',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 25 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_autopublish'
  $field_instances ['node-package_gallery-field_autopublish'] = array (
    'bundle' => 'package_gallery',
    'default_value' => array (
      0 => array (
        'value' => 0 
      ) 
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_autopublish',
    'label' => 'Use embargo and Publish Date',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (
        'display_label' => 1 
      ),
      'type' => 'options_onoff',
      'weight' => 11 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_autopublishdate'
  $field_instances ['node-package_gallery-field_autopublishdate'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_autopublishdate',
    'label' => 'Auto publish date',
    'required' => 0,
    'settings' => array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'date',
      'settings' => array (
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array (),
        'year_range' => '-3:+3' 
      ),
      'type' => 'date_select',
      'weight' => 12 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_byline'
  $field_instances ['node-package_gallery-field_byline'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 23 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_byline',
    'label' => 'Byline',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 2 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_concepts'
  $field_instances ['node-package_gallery-field_concepts'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 12 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 22 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_concepts',
    'label' => 'Concepts',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 19 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_copyright'
  $field_instances ['node-package_gallery-field_copyright'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 24 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_copyright',
    'label' => 'Copyright',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 10 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_destinations'
  $field_instances ['node-package_gallery-field_destinations'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_destinations',
    'label' => 'Destinations',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'dpidestinations',
      'settings' => array (),
      'type' => 'destination_default_widget',
      'weight' => 28 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_displaytitle'
  $field_instances ['node-package_gallery-field_displaytitle'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h1' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 2 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'big-title',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 4 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 1 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => '',
          'tag' => 'h1' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_displaytitle',
    'label' => 'Display Title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 1 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_editorialpublicationdate'
  $field_instances ['node-package_gallery-field_editorialpublicationdate'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 10 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 3 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 6 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_editorialpublicationdate',
    'label' => 'Editorial Publication date',
    'required' => 0,
    'settings' => array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'date',
      'settings' => array (
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array (),
        'year_range' => '-3:+3' 
      ),
      'type' => 'date_select',
      'weight' => 14 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_editorialupdatedate'
  $field_instances ['node-package_gallery-field_editorialupdatedate'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 4 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 4 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'medium',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 5 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_editorialupdatedate',
    'label' => 'Editorial Update Date',
    'required' => 0,
    'settings' => array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'date',
      'settings' => array (
        'increment' => 1,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array (),
        'year_range' => '-3:+3' 
      ),
      'type' => 'date_select',
      'weight' => 13 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_embededobjects'
  $field_instances ['node-package_gallery-field_embededobjects'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => TRUE,
          'context' => 'dpi_mediabox',
          'type_number' => 'all',
        ),
        'type' => 'media_box',
        'weight' => 17 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => FALSE,
          'context' => 'dpi_simpleimg',
        ),
        'type' => 'first_element',
        'weight' => 6 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => TRUE,
          'context' => 'dpi_simpleimg',
        ),
        'type' => 'first_element',
        'weight' => 2 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => TRUE,
          'type_links_list' => FALSE,
          'type_video' => FALSE,
          'context' => 'dpi_simpleimg',
        ),
        'type' => 'first_element',
        'weight' => 6 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_embededobjects',
    'label' => 'EmbededObjects',
    'required' => 0,
    'settings' => array (
      'field_to_analyse' => 'field_textbody',
      'referencable_types' => array (
        'image' => 'image',
        'video' => 'video',
        'atom_package' => 'atom_package' 
      ),
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'dpicontenttypes',
      'settings' => array (),
      'type' => 'dpiatom_reference_textfield',
      'weight' => 6 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_entities'
  $field_instances ['node-package_gallery-field_entities'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 14 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 25 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_entities',
    'label' => 'Entities',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 21 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_externalurl'
  $field_instances ['node-package_gallery-field_externalurl'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 21 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_externalurl',
    'label' => 'External URL',
    'required' => 0,
    'settings' => array (
      'attributes' => array (
        'class' => '',
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => '_blank',
        'title' => '' 
      ),
      'display' => array (
        'url_cutoff' => 80 
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'none',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'link',
      'settings' => array (),
      'type' => 'link_field',
      'weight' => 16 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_freetags'
  $field_instances ['node-package_gallery-field_freetags'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 19 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_freetags',
    'label' => 'Free Tags',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 22 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_iptc'
  $field_instances ['node-package_gallery-field_iptc'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 11 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 7 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 7 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'unfoldcompanionminimal',
        'settings' => array (),
        'type' => 'term_in_red',
        'weight' => 5 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_iptc',
    'label' => 'IPTC',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 18 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_linkslists'
  $field_instances ['node-package_gallery-field_linkslists'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => FALSE,
          'type_links_list' => TRUE,
          'type_video' => FALSE,
          'context' => 'full',          
        ),
        'type' => 'first_element',
        'weight' => 9 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'type_atom_package' => FALSE,
          'type_audio' => FALSE,
          'type_image' => FALSE,
          'type_links_list' => TRUE,
          'type_video' => FALSE,
          'context' => 'full',          
        ),
        'type' => 'first_element',
        'weight' => 9 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_linkslists',
    'label' => 'Links lists',
    'required' => 0,
    'settings' => array (
      'field_to_analyse' => 'field_textbody',
      'referencable_types' => array (
        'image' => 0,
        'links_list' => 'links_list',
        'video' => 0 
      ),
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'dpicontenttypes',
      'settings' => array (),
      'type' => 'dpiatom_reference_textfield',
      'weight' => 7 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_locations'
  $field_instances ['node-package_gallery-field_locations'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 13 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_locations',
    'label' => 'Locations',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 20 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_packagelayout'
  $field_instances ['node-package_gallery-field_packagelayout'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 20 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 7 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_packagelayout',
    'label' => 'Package Layout',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 17 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_packagetype'
  $field_instances ['node-package_gallery-field_packagetype'] = array (
    'bundle' => 'package_gallery',
    'default_value' =>  array (
      array (
        'tid' => 6
      ) 
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 27 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 8 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_packagetype',
    'label' => 'Package Type',
    'required' => 1,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (),
      'type' => 'options_select',
      'weight' => 27 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_persons'
  $field_instances ['node-package_gallery-field_persons'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 15 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_persons',
    'label' => 'Persons',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 24 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_restrictedaccess'
  $field_instances ['node-package_gallery-field_restrictedaccess'] = array (
    'bundle' => 'package_gallery',
    'default_value' => array (
      0 => array (
        'value' => 0 
      ) 
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 26 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_restrictedaccess',
    'label' => 'Restricted Access',
    'required' => 1,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (),
      'type' => 'options_select',
      'weight' => 27 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textbarette'
  $field_instances ['node-package_gallery-field_textbarette'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 3 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 3 
      ),
      'targetblock_red' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 8 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'cat',
          'tag' => 'span' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textbarette',
    'label' => 'Barette',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 4 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textbody'
  $field_instances ['node-package_gallery-field_textbody'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 16 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (
          'trim_length' => 400 
        ),
        'type' => 'text_trimmed',
        'weight' => 11 
      ),
      'targetblock_medium' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (
          'trim_length' => 300 
        ),
        'type' => 'text_trimmed',
        'weight' => 9 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'targetblock_small' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (
          'trim_length' => 150 
        ),
        'type' => 'text_trimmed',
        'weight' => 3 
      ),
      'teaser' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textbody',
    'label' => 'Body',
    'required' => 0,
    'settings' => array (
      'context' => 'full',
      'display_summary' => 0,
      'dnd_enabled' => 1,
      'mee_enabled' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 20,
        'summary_rows' => 5 
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 5 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textchapo'
  $field_instances ['node-package_gallery-field_textchapo'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'heading',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 7 
      ),
      'targetblock_big' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'description',
          'tag' => 'div' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 8 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'dpicontenttypes',
        'settings' => array (
          'class' => 'heading',
          'tag' => 'h2' 
        ),
        'type' => 'simple_html_tag_formatter',
        'weight' => 7 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textchapo',
    'label' => 'Chapo',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 5 
      ),
      'type' => 'text_textarea',
      'weight' => 3 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textforetitle'
  $field_instances ['node-package_gallery-field_textforetitle'] = array (
    'bundle' => 'package_gallery',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 1 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textforetitle',
    'label' => 'Fore title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 3 
      ),
      'type' => 'text_textarea',
      'weight' => 8 
    ) 
  );
  
  // Exported field_instance: 'node-package-field_textsubtitle'
  $field_instances ['node-package-field_textsubtitle'] = array (
    'bundle' => 'package',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 3 
      ),
      'targetblock_big' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 18 
      ),
      'targetblock_medium' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 17 
      ),
      'targetblock_red' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 15 
      ),
      'targetblock_small' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 16 
      ),
      'teaser' => array (
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 3 
      ) 
    ),
    'entity_type' => 'node',
    'field_name' => 'field_textsubtitle',
    'label' => 'Sub Title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 3 
      ),
      'type' => 'text_textarea',
      'weight' => 9 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-audio-scald_authors'
  $field_instances ['scald_atom-audio-scald_authors'] = array (
    'bundle' => 'audio',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_authors',
    'label' => 'Authors',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 3 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-audio-scald_tags'
  $field_instances ['scald_atom-audio-scald_tags'] = array (
    'bundle' => 'audio',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_tags',
    'label' => 'Tags',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 2 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-audio-scald_thumbnail'
  $field_instances ['scald_atom-audio-scald_thumbnail'] = array (
    'bundle' => 'audio',
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_thumbnail',
    'label' => 'Thumbnail',
    'required' => FALSE,
    'settings' => array (
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'thumbnails/audio',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'image',
      'settings' => array (
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber' 
      ),
      'type' => 'image_image',
      'weight' => - 4 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_byline'
  $field_instances ['scald_atom-image-field_byline'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 4 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_byline',
    'label' => 'Byline',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 0 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_caption'
  $field_instances ['scald_atom-image-field_caption'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 3 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_caption',
    'label' => 'Caption',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'rows' => 5 
      ),
      'type' => 'text_textarea',
      'weight' => 2 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_concepts'
  $field_instances ['scald_atom-image-field_concepts'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_concepts',
    'label' => 'Concepts',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 9 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_copyright'
  $field_instances ['scald_atom-image-field_copyright'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 8 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_copyright',
    'label' => 'Copyright',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 4 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_displaytitle'
  $field_instances ['scald_atom-image-field_displaytitle'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'hidden',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_displaytitle',
    'label' => 'Display Title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 6 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_entities'
  $field_instances ['scald_atom-image-field_entities'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 6 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_entities',
    'label' => 'Entities',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 11 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_freetags'
  $field_instances ['scald_atom-image-field_freetags'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 7 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_freetags',
    'label' => 'Free Tags',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 13 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_iptc'
  $field_instances ['scald_atom-image-field_iptc'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 5 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_iptc',
    'label' => 'IPTC',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 15 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_locations'
  $field_instances ['scald_atom-image-field_locations'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_locations',
    'label' => 'Locations',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 17 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_persons'
  $field_instances ['scald_atom-image-field_persons'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_persons',
    'label' => 'Persons',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 19 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-field_rating'
  $field_instances ['scald_atom-image-field_rating'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_rating',
    'label' => 'Rating',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (),
      'type' => 'options_select',
      'weight' => 7 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-scald_authors'
  $field_instances ['scald_atom-image-scald_authors'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 13 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_authors',
    'label' => 'Authors',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 3 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-scald_tags'
  $field_instances ['scald_atom-image-scald_tags'] = array (
    'bundle' => 'image',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_tags',
    'label' => 'Tags',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 2 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-image-scald_thumbnail'
  $field_instances ['scald_atom-image-scald_thumbnail'] = array (
    'bundle' => 'image',
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 14 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_thumbnail',
    'label' => 'Image',
    'required' => FALSE,
    'settings' => array (
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'thumbnails/image',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'image',
      'settings' => array (
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber' 
      ),
      'type' => 'image_image',
      'weight' => - 4 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_concepts'
  $field_instances ['scald_atom-links_list-field_concepts'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 6 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_concepts',
    'label' => 'Concepts',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 7 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_copyright'
  $field_instances ['scald_atom-links_list-field_copyright'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 7 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_copyright',
    'label' => 'Copyright',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 9 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_displaytitle'
  $field_instances ['scald_atom-links_list-field_displaytitle'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 4 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_displaytitle',
    'label' => 'Display Title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 1 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_entities'
  $field_instances ['scald_atom-links_list-field_entities'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 8 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_entities',
    'label' => 'Entities',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 11 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_freetags'
  $field_instances ['scald_atom-links_list-field_freetags'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 9 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_freetags',
    'label' => 'Free Tags',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 13 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_iptc'
  $field_instances ['scald_atom-links_list-field_iptc'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_iptc',
    'label' => 'IPTC',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 15 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_linkitems'
  $field_instances ['scald_atom-links_list-field_linkitems'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'link',
        'settings' => array (),
        'type' => 'link_default',
        'weight' => - 1 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_linkitems',
    'label' => 'Link Items',
    'required' => 0,
    'settings' => array (
      'attributes' => array (
        'class' => '',
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => 'default',
        'title' => '' 
      ),
      'display' => array (
        'url_cutoff' => 80 
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'optional',
      'title_maxlength' => 254,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'link',
      'settings' => array (),
      'type' => 'link_field',
      'weight' => 2 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-field_persons'
  $field_instances ['scald_atom-links_list-field_persons'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 11 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_persons',
    'label' => 'Persons',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 17 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-scald_authors'
  $field_instances ['scald_atom-links_list-scald_authors'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_authors',
    'label' => 'Authors',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 3 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-scald_tags'
  $field_instances ['scald_atom-links_list-scald_tags'] = array (
    'bundle' => 'links_list',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_tags',
    'label' => 'Tags',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 2 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-links_list-scald_thumbnail'
  $field_instances ['scald_atom-links_list-scald_thumbnail'] = array (
    'bundle' => 'links_list',
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_thumbnail',
    'label' => 'Thumbnail',
    'required' => FALSE,
    'settings' => array (
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'thumbnails/links_list',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'image',
      'settings' => array (
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber' 
      ),
      'type' => 'image_image',
      'weight' => - 4 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-twitter-scald_authors'
  $field_instances ['scald_atom-twitter-scald_authors'] = array (
    'bundle' => 'twitter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_authors',
    'label' => 'Authors',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 3 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-twitter-scald_tags'
  $field_instances ['scald_atom-twitter-scald_tags'] = array (
    'bundle' => 'twitter',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array (),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_tags',
    'label' => 'Tags',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => - 2 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-twitter-scald_thumbnail'
  $field_instances ['scald_atom-twitter-scald_thumbnail'] = array (
    'bundle' => 'twitter',
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_thumbnail',
    'label' => 'Thumbnail',
    'required' => FALSE,
    'settings' => array (
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'thumbnails/twitter',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'image',
      'settings' => array (
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber' 
      ),
      'type' => 'image_image',
      'weight' => - 4 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_concepts'
  $field_instances ['scald_atom-video-field_concepts'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_concepts',
    'label' => 'Concepts',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 4 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_copyright'
  $field_instances ['scald_atom-video-field_copyright'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_copyright',
    'label' => 'Copyright',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 2 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_displaytitle'
  $field_instances ['scald_atom-video-field_displaytitle'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 11 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_displaytitle',
    'label' => 'Display Title',
    'required' => 0,
    'settings' => array (
      'text_processing' => 1,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 0 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_entities'
  $field_instances ['scald_atom-video-field_entities'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 8 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_entities',
    'label' => 'Entities',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 6 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_freetags'
  $field_instances ['scald_atom-video-field_freetags'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 3 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_freetags',
    'label' => 'Free Tags',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 8 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_iptc'
  $field_instances ['scald_atom-video-field_iptc'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 5 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_iptc',
    'label' => 'IPTC',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 10 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_locations'
  $field_instances ['scald_atom-video-field_locations'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 10 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_locations',
    'label' => 'Locations',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 16 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_persons'
  $field_instances ['scald_atom-video-field_persons'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 7 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_persons',
    'label' => 'Persons',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 12 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-field_rating'
  $field_instances ['scald_atom-video-field_rating'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 6 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'field_rating',
    'label' => 'Rating',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (),
      'type' => 'options_select',
      'weight' => 14 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-scald_authors'
  $field_instances ['scald_atom-video-scald_authors'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 4 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_authors',
    'label' => 'Authors',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 18 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-scald_tags'
  $field_instances ['scald_atom-video-scald_tags'] = array (
    'bundle' => 'video',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 9 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_tags',
    'label' => 'Tags',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'taxonomy',
      'settings' => array (
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60 
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 19 
    ) 
  );
  
  // Exported field_instance: 'scald_atom-video-scald_thumbnail'
  $field_instances ['scald_atom-video-scald_thumbnail'] = array (
    'bundle' => 'video',
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'settings' => array (),
        'type' => 'hidden',
        'weight' => 12 
      ) 
    ),
    'entity_type' => 'scald_atom',
    'field_name' => 'scald_thumbnail',
    'label' => 'Thumbnail',
    'required' => FALSE,
    'settings' => array (
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'thumbnails/video',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'image',
      'settings' => array (
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber' 
      ),
      'type' => 'image_image',
      'weight' => 17 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_person_phonenumber'
  $field_instances ['taxonomy_term-persons-field_person_phonenumber'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 7 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_person_phonenumber',
    'label' => 'Phone number',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 8 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personbirthdate'
  $field_instances ['taxonomy_term-persons-field_personbirthdate'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'date',
        'settings' => array (
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '' 
        ),
        'type' => 'date_default',
        'weight' => 3 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personbirthdate',
    'label' => 'Birth date',
    'required' => 0,
    'settings' => array (
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'date',
      'settings' => array (
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array (),
        'year_range' => '1000:+0' 
      ),
      'type' => 'date_select',
      'weight' => 4 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personemail'
  $field_instances ['taxonomy_term-persons-field_personemail'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 5 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personemail',
    'label' => 'Email',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 6 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personfirstname'
  $field_instances ['taxonomy_term-persons-field_personfirstname'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personfirstname',
    'label' => 'Firstname',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 1 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_persongender'
  $field_instances ['taxonomy_term-persons-field_persongender'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'list',
        'settings' => array (),
        'type' => 'list_default',
        'weight' => 4 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_persongender',
    'label' => 'Gender',
    'required' => 0,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'options',
      'settings' => array (),
      'type' => 'options_select',
      'weight' => 5 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personisauthor'
  $field_instances ['taxonomy_term-persons-field_personisauthor'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'list',
        'settings' => array (),
        'type' => 'list_default',
        'weight' => 9 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personisauthor',
    'label' => 'List as author',
    'required' => FALSE,
    'settings' => array (
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'options',
      'settings' => array (
        'display_label' => 0 
      ),
      'type' => 'options_onoff',
      'weight' => 10 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personlastname'
  $field_instances ['taxonomy_term-persons-field_personlastname'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 1 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personlastname',
    'label' => 'Lastname',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 2 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personnickname'
  $field_instances ['taxonomy_term-persons-field_personnickname'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 2 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personnickname',
    'label' => 'Nickname',
    'required' => 0,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 3 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personpicture'
  $field_instances ['taxonomy_term-persons-field_personpicture'] = array (
    'bundle' => 'persons',
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'image',
        'settings' => array (
          'image_link' => '',
          'image_style' => '' 
        ),
        'type' => 'image',
        'weight' => 8 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personpicture',
    'label' => 'Picture',
    'required' => 0,
    'settings' => array (
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'personpicture',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'active' => 1,
      'module' => 'image',
      'settings' => array (
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'bar' 
      ),
      'type' => 'image_image',
      'weight' => 9 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-persons-field_personwebsite'
  $field_instances ['taxonomy_term-persons-field_personwebsite'] = array (
    'bundle' => 'persons',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'link',
        'settings' => array (),
        'type' => 'link_default',
        'weight' => 6 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_personwebsite',
    'label' => 'Website',
    'required' => 0,
    'settings' => array (
      'attributes' => array (
        'class' => '',
        'configurable_title' => 0,
        'rel' => 'nofollow',
        'target' => '_blank',
        'title' => '' 
      ),
      'display' => array (
        'url_cutoff' => 80 
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1 
    ),
    'widget' => array (
      'active' => 0,
      'module' => 'link',
      'settings' => array (),
      'type' => 'link_field',
      'weight' => 7 
    ) 
  );
  
  // Exported field_instance: 'taxonomy_term-scald_authors-scald_author_url'
  $field_instances ['taxonomy_term-scald_authors-scald_author_url'] = array (
    'bundle' => 'scald_authors',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Homepage of the Author',
    'display' => array (
      'default' => array (
        'label' => 'above',
        'module' => 'text',
        'settings' => array (),
        'type' => 'text_default',
        'weight' => 0 
      ) 
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'scald_author_url',
    'label' => 'URL',
    'required' => FALSE,
    'settings' => array (
      'text_processing' => 0,
      'user_register_form' => FALSE 
    ),
    'widget' => array (
      'module' => 'text',
      'settings' => array (
        'size' => 60 
      ),
      'type' => 'text_textfield',
      'weight' => 1 
    ) 
  );
  
  // Translatables
  // Included for use with string extractors like potx.
  t ( 'Authors' );
  t ( 'Auto publish date' );
  t ( 'Barette' );
  t ( 'Birth date' );
  t ( 'Body' );
  t ( 'Byline' );
  t ( 'Caption' );
  t ( 'Chapo' );
  t ( 'Comment' );
  t ( 'Concepts' );
  t ( 'Copyright' );
  t ( 'Destinations' );
  t ( 'Display Title' );
  t ( 'Editorial Publication date' );
  t ( 'Editorial Update Date' );
  t ( 'Email' );
  t ( 'EmbededObjects' );
  t ( 'Entities' );
  t ( 'External URL' );
  t ( 'Firstname' );
  t ( 'Fore title' );
  t ( 'Free Tags' );
  t ( 'Gender' );
  t ( 'Homepage of the Author' );
  t ( 'IPTC' );
  t ( 'Image' );
  t ( 'Lastname' );
  t ( 'Link Items' );
  t ( 'Links lists' );
  t ( 'List as author' );
  t ( 'Locations' );
  t ( 'Nickname' );
  t ( 'Package Layout' );
  t ( 'Package Type' );
  t ( 'Persons' );
  t ( 'Phone number' );
  t ( 'Picture' );
  t ( 'Rating' );
  t ( 'Restricted Access' );
  t ( 'Sub Title' );
  t ( 'Tags' );
  t ( 'Thumbnail' );
  t ( 'URL' );
  t ( 'Use embargo and Publish Date' );
  t ( 'Website' );
  
  return $field_instances;
}
