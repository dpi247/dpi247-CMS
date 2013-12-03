(function ($) {

  Drupal.behaviors.dpi_vertical_tabs = {
    attach: function(context, settings) {
      $('.dpi_vertical_tabs .dpi_vertical_tabs_content').hide();
      $('.dpi_vertical_tabs_pane_title').hide();
      // On click
      $('.dpi_vertical_tabs_option > a').click(function(e){
        // Select tab item
        self.focus();
        // Reset all elements
        $('.dpi_vertical_tabs .dpi_vertical_tabs_content').hide();
        $('.dpi_vertical_tabs_option').removeClass('dpi_vertical_tabs_active');
        // Show expected pane and tab
        $(this).parent().addClass("dpi_vertical_tabs_active");
        $(this.hash).show();
        return false;
      });
      // Enable one tab on page load
      var default_tab = '#dpi_vertical_tabs_option_0';
      if(document.location.hash.contains('dpi_vertical_tabs')){
        default_tab = document.location.hash.replace('content','option');
      }
      $(default_tab + ' > a').click();
    }
  };
})(jQuery);