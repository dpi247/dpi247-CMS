(function ($) {
  // Documentation on Drupal JavaScript behaviors can be found here: http://drupal.org/node/114774#javascript-behaviors
  Drupal.behaviors.reportabuse = {
    attach: function(context){
      $('.dpicomments-reportabuse-widget', context).once('reportabuse', function(){
        var dpicomments_reportabsue_widget = $(this);
        dpicomments_reportabsue_widget.find('.reportabuse-link').attr('href', function(){ return $(this).attr('href') + '&json=true'; }).click(function(){
          $.getJSON($(this).attr('href'), function(json){
            if (json) {
              var newWidget = $(json.widget);
              newWidget.hide();
              dpicomments_reportabsue_widget.replaceWith(newWidget);
              newWidget.fadeIn('slow');
              Drupal.attachBehaviors();
            }
          });
          return false;
        });
      });
    }
  };
})(jQuery)
