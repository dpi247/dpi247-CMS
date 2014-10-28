<?php

/**
 * @file
 * This file contains declartion for admin form.
 */

/**
 * Menu callback to configure module settings.
 */
function dpicomments_settings() {
  $form['dpicomments_global_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dpicomments global settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['dpicomments_global_settings']['dpicomments_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include standard dpicomments js file.'),
    '#default_value' => variable_get('dpicomments_add_js', 1),
  );

  $form['dpicomments_comment_widget_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment report abuse widget settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_widget_show'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add report abuse voting widget to comments"),
    '#default_value' => variable_get('dpicomments_comment_widget_show', 0),
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Vote text"),
    '#default_value' => variable_get('dpicomments_comment_vote_text', t('Vote')),
    '#description' => t('Enter, for example, <em>Vote</em> or <em>Digg</em>.'),
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_undo_vote'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to undo their comment votes"),
    '#default_value' => variable_get('dpicomments_comment_undo_vote', 0),
  );

  $form['dpicomments_comment_widget_settings']['dpicomments_comment_undo_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Feedback provided to voter when he already voted and user CAN undo his vote"),
    '#default_value' => variable_get('dpicomments_comment_undo_vote_text', ''),
    '#description' => t('Enter, for example, <em>Undugg</em>, <em>Undo vote</em>, or <strong>leave it blank to provide user with "arrow down" icon.</strong>'),
    '#states' => array(
      'visible' => array(
        'input[name="dpicomments_comment_undo_vote"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['array_filter'] = array('#type' => 'hidden');

  return system_settings_form($form);
}
