<?php

/**
 * Returns a Google line chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/linechart
 *
 * @param $data
 *   A 2d table of data.
 * @param $title
 * 	 The title of graph.
 * @param $pointSize
 *   The size of the point in the graph.
 * @param $AxeXTitle
 *   Title of the X axe
 * @param $width
 *   The width of graph.
 * @param $height
 *   The height of graph.
 * @param $jsprint
 *   Wether the js as to be printed or not
 *
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_dpicommons_line_chart($vars) {
  static $i = 0;

  $data = $vars['data'];
  $title = $vars['title'];
  $pointSize = $vars['pointSize'];
  $AxeXTitle = $vars['AxeXTitle'];
  $width = $vars['width'];
  $height = $vars['height'];
  $jsprint = $vars['jsprint'];

  // Include Google API JS
  drupal_add_js(drupal_get_path('module', 'dpicommons').'/js/googleChartApi.js');
  // Google API takes datas in JSON table
  $data = json_encode($data);

  $function_name = 'drawChartLine'.$i;
	$jsCode = '';
	if ($jsprint) {
	  $jsCode .= '
	    <script type="text/javascript">
	      drawFunctions.push("'.$function_name.'");';
	} else {
    $jsCode .= '
        (function($) {
          $(document).ready(function() {
            setTimeout(function(){google.load("visualization", "1", {"callback":"console.log()", "packages":["corechart"]})}, 1);
            google.setOnLoadCallback('.$function_name.');';
  }
	$jsCode .= '
      	    function '.$function_name.'() {
              var data = google.visualization.arrayToDataTable('.$data.');

              var options = {
                title: "'.$title.'",
                pointSize: '.$pointSize.',
                hAxis: {title: "'.$AxeXTitle.'"}
              };

              var chart = new google.visualization.LineChart(document.getElementById("chart_div_'.$i.'"));
              chart.draw(data, options);
            }';
	if ($jsprint) {
	  $jsCode .= '
	    </script>';
	} else {
    $jsCode .= '
          });
        })(jQuery);';
  }

	$output = dpicommons_theme_chart_div('chart_div_'.$i, $width, $height);
	// Include the js code in the page
	if ($jsprint) {
	  $output .= $jsCode;
	} else {
	  drupal_add_js($jsCode, 'inline');
	}

	$i++;

	return $output;
}

/**
 * Returns a Google pie chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/piechart
 *
 * Example:
 *  $data=array(
 *    array("DDU", 20000),
 *    array("ODM", 22000),
 *    array("ASL", 18000),
 *    array("JPL", 8000),
 *  );
 *  $vars['pie']=theme("dpicommons_pie_chart",$data,"Number of commit","Dev name","commits","400","400");
 *
 * @param $data
 *   An array each element is a  2d (string, integer) array.
 * @param $title
 * 	 The title of graph.
 * @param $header_name
 * 	 The name of the value.
 * @param $header_unit
 * 	 The unit of the value
 * @param $width
 *   The width of graph.
 * @param $height
 *   The height of graph.
 * @param $jsprint
 *   Wether the js as to be printed or not
 *
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_dpicommons_pie_chart($vars) {
  static $i = 0;

  $data = $vars['data'];
  $title = $vars['title'];
  $header_name = $vars['header_name'];
  $header_unit = $vars['header_unit'];
  $width = $vars['width'];
  $height = $vars['height'];
  $jsprint = $vars['jsprint'];

  // Include Google API JS
  drupal_add_js(drupal_get_path('module', 'dpicommons').'/js/googleChartApi.js');
  // Google API takes datas in JSON table
  $data = json_encode($data);

  $function_name = 'drawChartPie'.$i;
  $jsCode = '';
  if ($jsprint) {
    $jsCode .= '
	    <script type="text/javascript">
	      drawFunctions.push("'.$function_name.'");';
  } else {
    $jsCode .= '
        (function($) {
          $(document).ready(function() {
            setTimeout(function(){google.load("visualization", "1", {"callback":"console.log()", "packages":["corechart"]})}, 1);
            google.setOnLoadCallback('.$function_name.');';
  }
  $jsCode .= '
      	    function '.$function_name.'() {
              // Set chart options
              var data = new google.visualization.DataTable();
              data.addColumn("string", "'.$header_name.'");
              data.addColumn("number", "'.$header_unit.'");
              data.addRows('.$data.');

              var options = {
                  "title":"'.$title.'",
              };

              var chart = new google.visualization.PieChart(document.getElementById("piechart_div_'.$i.'"));
              chart.draw(data, options);
            }';
  if ($jsprint) {
    $jsCode .= '
	    </script>';
  } else {
    $jsCode .= '
          });
        })(jQuery);';
  }

  $output = dpicommons_theme_chart_div('piechart_div_'.$i, $width, $height);
	// Include the js code in the page
	if ($jsprint) {
	  $output .= $jsCode;
	} else {
	  drupal_add_js($jsCode, 'inline');
	}

  $i++;

  return $output;
}

/**
 * Returns a Google bar chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/barchart#Example
 *
 * Example:
 *  $data=array(
 *    array("Month", "DDU","ODM","ASL","JPL"),
 *    array("Sept", 4786, 4356, 2345, 000),
 *    array("Oct", 3456, 4321, 3456, 000),
 *    array("Nov", 2134, 6543, 4563, 2000),
 *  );
 *  $vars['bar']=theme("dpicommons_bar_chart",$data,"Number of commit","commits","Dev name","400","400");
 *
 * @param $data
 *   An matrix.
 *   The first line are the labels
 *   The first row represent how data are group together.
 *   Each other row represent a category of data
 * @param $title
 * 	 The title of graph.
 * @param $hAxis
 * 	 Horizontal Axis Label
 * @param $vAxis
 * 	 Vertical Axis Label
 * @param $width
 *   The width of graph.
 * @param $height
 *   The height of graph.
 * @param $jsprint
 *   Wether the js as to be printed or not
 *
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_dpicommons_bar_chart($vars) {
  static $i = 0;

  $data = $vars['data'];
  $title = $vars['title'];
  $hAxis = $vars['hAxis'];
  $vAxis = $vars['vAxis'];
  $width = $vars['width'];
  $height = $vars['height'];
  $jsprint = $vars['jsprint'];

  // Include Google API JS
  drupal_add_js(drupal_get_path('module', 'dpicommons').'/js/googleChartApi.js');
  // Google api takes datas in JSON table
  $data = json_encode($data);

  static $first_load = TRUE;
  if ($first_load) {
    drupal_add_js('
          (function($) {
            $(document).ready(function() {
              setTimeout(function(){google.load("visualization", "1", {"callback":"console.log()", "packages":["corechart"]})}, 1);
            });
          })(jQuery);', 'inline');
    $first_load = FALSE;
  }

  $function_name = 'drawChartBar'.$i;
  $jsCode = '';
  if ($jsprint) {
    $jsCode .= '
	    <script type="text/javascript">
	      drawFunctions.push("'.$function_name.'");';
  } else {
    $jsCode .= '
        (function($) {
          $(document).ready(function() {
            setTimeout(function(){google.load("visualization", "1", {"callback":"console.log()", "packages":["corechart"]})}, 1);
            google.setOnLoadCallback('.$function_name.');';
  }
  $jsCode .= '
      	    function '.$function_name.'() {
              // Set chart options
              var data = google.visualization.arrayToDataTable('.$data.');

              var options = {"title":"'.$title.'",
                              vAxis: {title: "'.$vAxis.'"},
                              hAxis: {title: "'.$hAxis.'"},
              };

              var chart = new google.visualization.BarChart(document.getElementById("barchart_div_'.$i.'"));
              chart.draw(data, options);
            }';
  if ($jsprint) {
    $jsCode .= '
	    </script>';
  } else {
    $jsCode .= '
          });
        })(jQuery);';
  }

  $output = dpicommons_theme_chart_div('barchart_div_'.$i, $width, $height);
  // Include the js code in the page
  if ($jsprint) {
    $output .= $jsCode;
  } else {
    drupal_add_js($jsCode, 'inline');
  }

  $i++;

  return $output;
}

/**
 * Returns a Google gauge chart.
 * see https://google-developers.appspot.com/chart/interactive/docs/gallery/barchart#Example
 *
 * Example:
 *  $data=array(
 *    array("ammount of commit", "DDU","ODM","ASL","JPL"),
 *    array("In percent", 30, 30, 30, 10),
 *  );
 *  $vars['bar']=theme("dpicommons_gauge_chart",$data,"Number of commit","commits","Dev name","400","400");
 *
 * @param $data
 *   An matrix.
 *   The first line are the labels
 *   The second line is the percentage:  *TOTAL SHOULD BE 100 ! On only one line
 *   No third line
 * @param $title
 * 	 The title of graph.
 * @param $hAxis
 * 	 Horizontal Axis Label
 * @param $vAxis
 * 	 Vertical Axis Label
 * @param $width
 *   The width of graph.
 * @param $height
 *   The height of graph.
 * @param $jsprint
 *   Wether the js as to be printed or not
 *
 * @return $output
 * 	 A div where the graph was displayed.
 */
function theme_dpicommons_gauge_chart($vars) {
  static $i = 0;

  $data = $vars['data'];
  $title = $vars['title'];
  $hAxis = $vars['hAxis'];
  $vAxis = $vars['vAxis'];
  $width = $vars['width'];
  $height = $vars['height'];
  $jsprint = $vars['jsprint'];

  // Include Google API JS
  drupal_add_js(drupal_get_path('module', 'dpicommons').'/js/googleChartApi.js');
  // Google api takes datas in JSON table
  $data = json_encode($data);

  static $first_load = TRUE;
  if ($first_load) {
    drupal_add_js('
          (function($) {
            $(document).ready(function() {
              setTimeout(function(){google.load("visualization", "1", {"callback":"console.log()", "packages":["corechart"]})}, 1);
            });
          })(jQuery);', 'inline');
    $first_load = FALSE;
  }
  
  $function_name = 'drawChartGauge'.$i;
  $jsCode = '';
  if ($jsprint) {
    $jsCode .= '
	    <script type="text/javascript">
	      drawFunctions.push("'.$function_name.'");';
  } else {
    $jsCode .= '
        (function($) {
          $(document).ready(function() {
            setTimeout(function(){google.load("visualization", "1", {"callback":"console.log()", "packages":["corechart"]})}, 1);
            google.setOnLoadCallback('.$function_name.');';
  }
  $jsCode .= '
	          function '.$function_name.'() {
              var data = google.visualization.arrayToDataTable('.$data.');

              var options = {
                title:"'.$title.'",
                vAxis: {title: "'.$vAxis.'"},
                hAxis: {title: "'.$hAxis.'",maxValue:"100"},
                isStacked: "True"
              }

              var chart = new google.visualization.BarChart(document.getElementById("gaugechart_div_'.$i.'"));
              chart.draw(data, options);
            }';
  if ($jsprint) {
    $jsCode .= '
	    </script>';
  } else {
    $jsCode .= '
          });
        })(jQuery);';
  }

  $output = dpicommons_theme_chart_div('gaugechart_div_'.$i, $width, $height);
  // Include the js code in the page
  if ($jsprint) {
    $output .= $jsCode;
  } else {
    drupal_add_js($jsCode, 'inline');
  }

  $i++;

  return $output;
}

function dpicommons_theme_chart_div($id, $width = NULL, $height = NULL) {
  $style = '';
  if ($width || $height) {
    $style .= ' style="';
    $style .= $width ? 'width:'.$width.';' : '';
    $style .= $height ? 'height:'.$height.';' : '';
    $style .= '"';
  }
  return '<div id="'.$id.'"'.$style.'></div>';
}
