Drupal.behaviors.pressFiltersExportConfig = {
	attach: function(context) {
		$("#export-loading").hide();
		$("#export-submit").click(function () {
			$("#export-loading").show();
			updateExport();
		});
	},
  detach: function(contact) {}
};

function updateExport() {
	var found = false;
	$("input[name='select_filters']").each(function() {
		if ($(this).attr('checked')) {
			found = true;
			var exportFilter = $(this).attr('value');
			$.ajax({
				type: "GET",
				url: "/dpipressfilters/config/export/"+exportFilter,
				cache: true,
				complete: function(data) {
					$("#export-area").text(data.responseText);
					$("#export-loading").hide();
				}
			});
			return;
		}
	});
	if (!found) {
		$("#export-area").html('No filter selected');
		$("#export-loading").hide();
	}
}
