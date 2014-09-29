<?php if (isset($data['#content']['nbviews'])) :?>
  <div class="dpiDashboardSocialCountAll" id="dpiDashboardSocialCountAllviews"><?php print $data['#content']['nbviews'] ?></div>
<?php endif;?>
<?php if (isset($data['#content']['nbcomments'])) :?>
  <div class="dpiDashboardSocialCountAll" id="dpiDashboardSocialCountAllcomments"><?php print $data['#content']['nbcomments'] ?></div>
<?php endif;?>
<?php if (isset($data['#content']['facebook'])) :?>
  <div class="dpiDashboardSocialCountAll" id="dpiDashboardSocialCountAllfacebook"><span id="dpiDashboardSocialCountAllfacebookDisplay"></span></div>
<?php endif;?>
<?php if (isset($data['#content']['twitter'])) :?>
  <div class="dpiDashboardSocialCountAll" id="dpiDashboardSocialCountAlltwitter"><span id="dpiDashboardSocialCountAlltwitterDisplay"></span></div>
<?php endif;?>
<?php if (isset($data['#content']['linkedin'])) :?>
  <div class="dpiDashboardSocialCountAll" id="dpiDashboardSocialCountAlllinkedin"><span id="dpiDashboardSocialCountAlllinkedinDisplay"></span></div>
<?php endif;?>
<?php if (isset($data['#content']['googleplus'])) :?>
  <div class="dpiDashboardSocialCountAll" id="dpiDashboardSocialCountAllgoogleplus"><span id="dpiDashboardSocialCountAllgoogleplusDisplay"></span></div>
<?php endif;?>