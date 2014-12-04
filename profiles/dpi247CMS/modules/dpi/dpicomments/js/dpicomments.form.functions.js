
/**
 * Check textarea max length
 */
function textareaMaxLength(field, evt, limit) {
  var evt = (evt) ? evt : event;
  var charCode =
    (typeof evt.which != "undefined") ? evt.which :
   ((typeof evt.keyCode != "undefined") ? evt.keyCode : 0);

  if (!(charCode >= 13 && charCode <= 126)) {
    return true;
  }

  return (field.value.length < limit);
}