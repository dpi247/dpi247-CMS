<?php
/**
 * @file
 * unfoldcompanion.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function unfoldcompanion_taxonomy_default_vocabularies() {
  return array(
    'destinations' => array(
      'name' => 'Destinations',
      'machine_name' => 'destinations',
      'description' => 'Destinations represent the structure of your editorial content.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'scald_authors' => array(
      'name' => 'Authors',
      'machine_name' => 'scald_authors',
      'description' => 'Scald authors vocabulary',
      'hierarchy' => 0,
      'module' => 'scald',
      'weight' => -5,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
    'scald_tags' => array(
      'name' => 'Scald tags',
      'machine_name' => 'scald_tags',
      'description' => 'Scald tags vocabulary',
      'hierarchy' => 0,
      'module' => 'scald',
      'weight' => -5,
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
