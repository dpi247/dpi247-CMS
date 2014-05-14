<?php

/**
 * Transform a search result in a usable format for templates
 *
 * @param array $vars
 */
function dpidamwidgets_preprocess_damsearch_render_result(&$vars) {
  $result = $vars['result'];

  if(isset($result->pagePackageId[0]) && !empty($result->pagePackageId[0])) {
    $thumbnail_name = $result->pagePackageId[0];
  } elseif (isset($result->img) && !empty($result->img)) {
    $dirty = $result->img;
    $pieces = explode("_", $dirty);
    $thumbnail_name = $pieces[0];
  } else {
    $thumbnail_name = false;
  }

  $get_thumb_url = dpi_variable_get('dpidam_search_thumbnails_URL', '');
  if($thumbnail_name && $get_thumb_url) {
    $small = _dpidam_get_dam_image($thumbnail_name, 'thS');
    $medium = _dpidam_get_dam_image($thumbnail_name, 'thM');
    $big = _dpidam_get_dam_image($thumbnail_name, 'high');
    $vars['thumbnail']['small'] = '<img src="/'.$small["filepath"].'" />';
    $vars['thumbnail']['medium'] = '<img src="/'.$medium["filepath"].'" />';
    $vars['thumbnail']['big'] = '<img src="/'.$big["filepath"].'" />';
  }

  $vars['prodId'] = (is_array($result->prodId)) ? $result->prodId : FALSE;
  $vars['parutions'] = (is_array($result->parutions)) ? $result->parutions : FALSE;
  $vars['zone'] = (is_array($result->zone)) ? $result->zone : FALSE;
  $vars['newsroom'] = (is_array($result->newsroom)) ? $result->newsroom : FALSE;

  $url = FALSE;
  if ($url) {
  } else {
    $url = '/archive/'.$result->pId[0];
  }

  // Add the page path and query to the result URL to get them as a context on the result page
  $parsed_url = parse_url($url);
  $url .= isset($parsed_url['query']) ? '&' : '?';
  $url .= 'referer='.urlencode($_SERVER['REQUEST_URI']);
  $vars['url'] = $url;

  $vars['title'] = (isset($result->title) && !empty($result->title)) ? $result->title : t('No title');
  $vars['byline'] = (isset($result->byline) && !empty($result->byline)) ? $result->byline : t('No byline');
  $vars['snippet'] = (isset($result->preview) && !empty($result->preview)) ? $result->preview : '';
  $vars['info'] = $result->pDate[0];

  if ($vars['thumbnail']['small']) {
    $vars['img'] = $vars['thumbnail']['small'];
  } else {
    $vars['img'] = '';
  }
}

function dpidamwidgets_preprocess_damsearch_render_facet(&$vars) {
  $facet = $vars["facet"];

  $vars['facet_dname'] = $facet->dname;
  $vars['facet_url'] = $facet->url['url'];
  $vars['facet_active'] = $facet->active;

  drupal_add_css(drupal_get_path('module', 'dpidamwidgets') . '/css/dpidamwidgets.css');
}

function dpidamwidgets_preprocess_damsearch_render_facet_term(&$vars) {
  $term = $vars['term'];
  $vars['term_display'] = $term->display;
  $vars['term_frequency'] = $term->freq;
  $vars['term_url'] = $term->url["url"];

  if (isset($term->active)) {
    $vars['term_active'] = $term->active;
  } else {
    $vars['term_active'] = FALSE;
  }

  drupal_add_css(drupal_get_path('module', 'dpidamwidgets') . '/css/dpidamwidgets.css');
}

function dpidamwidgets_preprocess_damsearch_render_pagination(&$vars) {
  $pagination = $vars['pagination'][0];

  $vars['current_page'] = $pagination->current;
  $vars['totalresults'] = $pagination->numFound;
  if (isset($pagination->prev)) $vars['prev'] = $pagination->prev;
  if (isset($pagination->next)) $vars['next'] = $pagination->next;
  if (isset($pagination->first)) $vars['first'] = $pagination->first;
  if (isset($pagination->last)) $vars['last'] = $pagination->last;
  $vars['pages'] = $pagination->pages;

  drupal_add_css(drupal_get_path('module', 'dpidamwidgets') . '/css/dpidamwidgets.css');
}
