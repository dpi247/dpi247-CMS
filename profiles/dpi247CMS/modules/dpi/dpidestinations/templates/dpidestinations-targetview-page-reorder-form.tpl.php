<?php

/**
 * @file
 * Default theme implementation to configure blocks.
 *
 * Available variables:
 * - $block_regions: An array of regions. Keyed by name with the title as value.
 * - $node_listing: An array of blocks keyed by region and then delta.
 * - $form_submit: Form submit button.
 *
 * Each $node_listing[$region] contains an array of blocks for that region.
 *
 * Each $data in $node_listing[$region] contains:
 * - $data->region_title: Region title for the listed block.
 * - $data->block_title: Block title.
 * - $data->region_select: Drop-down menu for assigning a region.
 * - $data->weight_select: Drop-down menu for setting weights.
 * - $data->configure_link: Block configuration link.
 * - $data->delete_link: For deleting user added blocks.
 *
 * @see template_preprocess_block_admin_display_form()
 * @see theme_block_admin_display()
 *
 * @ingroup themeable
 */
?>
<?php
  // Add table javascript.
drupal_add_js('misc/tableheader.js');
drupal_add_js(drupal_get_path('module', 'block') . '/block.js');

drupal_add_tabledrag('targetview', 'order', 'sibling', 'node-weight');

?>
<table id="targetview" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Node'); ?></th>
      <th><?php print t('Publication Date'); ?></th>
      <th><?php print t('Author'); ?></th>
      <th><?php print t('Weight'); ?></th>
      <th ><?php print t('Operations'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
      <?php foreach ($node_listing as $delta => $data): ?>
      <?php dsm($data);?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?>">
        <td class="node"><?php print $data->node_title; ?></td>
        <td><?php print $data->publication_date; ?></td>
        <td><?php print $data->author; ?></td>
        <td><?php print $data->weight_select; ?></td>
        
        <td><?php print $data->actions; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
  </tbody>
</table>

<?php print $form_submit; ?>
