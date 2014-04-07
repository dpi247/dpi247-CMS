/**
 * @file
 *   Provides the JavaScript behaviors for the cropings editor.
 */

(function($) {
	var browser = 'other';
	$.each($.browser, function(i, val) {
		if (i == "chrome" && val == true) {
			browser = "chrome";
		}
	});

	var currentAc = '';
	var ac = Drupal.settings.dpicontenttypesCropings;
	var modalOffset = 0;

	Drupal.behaviors.wallycontenttypesCropings = {
		attach: function(context, settings) {
			$("#dpicontenttypes-form-edit-crop-form").ready(function() {
				$("#dpicontenttypes-form-edit-crop-form").once('dpicontenttypes-cropings', function() {
					var timer = setInterval(function() {
						if (typeof $("#cropbox").attr("id") != "undefined" && $("#cropbox").width() > 0) {
							clearInterval(timer);

							var api = $.Jcrop($("#cropbox"),{
								onSelect: showCoordsAndSave,
								bgColor: "black",
								bgOpacity: .4,
								boxWidth: 800
							});

							// A handler to kill the action
							function nothing(e) {
								e.stopPropagation();
								e.preventDefault();
								return false;
							};

							var ratio = {};
							for(i in ac) {
								ratio[i] = ac[i][4];
							}

							// Returns event handler for animation callback
							function anim_handler(accc, ratio, i) {
								return function(e) {
									// Change class of selected button
									if (currentAc) {
										$("#"+currentAc).removeClass("crop_selected");
									}
									$("#"+i).addClass("crop_selected");	
									currentAc = i;
	
									api.animateTo(accc);
									api.setOptions({aspectRatio: ratio});
	
									return nothing(e);
								};
							};

							// Attach respective event handlers and charge default values in the form
							for(i in ac) {
								$("#"+i).click(anim_handler(ac[i], ratio[i], i));
							}

							// Select the first preset
							var first = true;
							for(i in ac) {
								if (first) {
									$("#"+i).trigger("click");
									first = false;
								}
							}

							// Fix the position of the modal window
							$('#modalContent').css('position', 'fixed');
							$('#modalContent').css('top', '60px');
						}
					}, 1);
				});
			});
		}
	};

	function showCoordsAndSave(c) {
		if (currentAc) {
			// 1 ajouté la fin de l'array pour flager la modification de ce preset
			var serialCoord = [c.x, c.y, c.w, c.h, 1];
			$("#" + currentAc + "_serialCoord").val(serialCoord);
			ac[currentAc][0] = c.x;
			ac[currentAc][1] = c.y;
			ac[currentAc][2] = c.x2;
			ac[currentAc][3] = c.y2;
		}
	};
})(jQuery);
