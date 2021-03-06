/**
 * @file
 *   Provides the JavaScript behaviors for the Atom Reference field.
 */
(function($) {
Drupal.behaviors.dpiatom_reference = {
  attach: function(context) {
    $("div.dpiatom_reference_drop_zone:not(.dpiatom_reference_processed)").each(function() {
      var $this = $(this);
      var $reset = $("<input type='button' />")
        .val(Drupal.t('Delete'))
        .click(function() {
          $(this)
            .hide()
            .closest('div.form-item')
            .find('input:text')
            .val('')
            .end()
            .find('div.dpiatom_reference_drop_zone')
            .empty()
            .append(Drupal.t('Drop a resource here'));
          // Remove the "edit croping" link
          $(this)
            .closest('td')
            .find('a.ctools-use-modal')
            .empty();
        });
      // If the element doesn't have a value yet, hide the Delete button
      // by default
      if (!$this.closest('div.form-item').find('input:text').val()) {
        $reset.css('display', 'none');
      }
      $this
        .addClass('dpiatom_reference_processed')
        .bind('dragover', function(e) {e.preventDefault();})
        .bind('dragenter', function(e) {e.preventDefault();})
        .bind('drop', function(e) {
          var dt = e.originalEvent.dataTransfer.getData('Text').replace(/^\[scald=(\d+).*$/, '$1');
          var ret = Drupal.dpiatom_reference.droppable(dt, this);
          var $this = $(this);
          if (ret.found && ret.keepgoing) {
            $this
              .empty()
              .append(Drupal.dnd.Atoms[dt].editor)
              .closest('div.form-item')
              .find('input:text')
              .val(dt)
              .end()
              .find('input:button')
              .show();
            if (ret.type == 'image') {
	            // Add the "edit croping" link
	            var entityType = Drupal.settings.dpicontenttypesCropingsLinks.entityType;
	            var eid = Drupal.settings.dpicontenttypesCropingsLinks.eid;
	            var aid_regex_matches = e.originalEvent.dataTransfer.getData('Text').match(/^\[scald=(\d+).*$/);
	            var aid = aid_regex_matches[1];
	            var editCropLink = '/admin/editdpicrop/ajax/' + entityType + '/' + eid + '/' + aid;
	            var editCropLabel = Drupal.t('Click here to edit croping informations');
	            $this
	              .closest('div.form-item')
	              .after('<a href="' + editCropLink + '" class="ctools-use-modal" title="">' + editCropLabel + '</a>');
	            // Bind the modal behavior
            }
            Drupal.behaviors.ZZCToolsModal.attach(document);
          }
          else {
            var placeholder = Drupal.t("You can't drop a resource of type %type in this field", {'%type': ret.type});
            $this.empty().append(placeholder);
          }
          e.stopPropagation();
          e.preventDefault();
          return false;
        })
        .closest('div.form-item')
        .find('input')
        .css('display', 'none')
        .end()
        .append($reset);
    });
  }
}

if (!Drupal.dpiatom_reference) {
  Drupal.dpiatom_reference = {};
  Drupal.dpiatom_reference.droppable = function(ressource_id, field) {
    var retVal = {'keepgoing': true, 'found': true};
    if (Drupal.dnd.Atoms[ressource_id]) {
      var type = Drupal.dnd.Atoms[ressource_id].meta.type;
      var accept = $(field).closest('div.form-item').find('input:text').attr('data-types').split(',');
      if (jQuery.inArray(type, accept) == -1) {
        retVal.keepgoing = false;
      }
      retVal.type = type;
    }
    else {
      retVal.found = false;
    }
    return retVal;
  }
}
})(jQuery);
