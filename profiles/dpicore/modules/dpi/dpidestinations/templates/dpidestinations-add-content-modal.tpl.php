<?php
/**
 * @file
 * Template to control the add content modal.
 */
?>
<style type="text/css">
.destinations-add-content-modal{
height: 100%;
}
 .destinations-add-content-modal ul.tabs{
background-color: white;
height:40px;
margin: 0;
padding:0;
margin-top:5px;
margin-right:20px;


}
.panels-add-content-modal{
border-top: 1px solid #ccc; 
}
.destinations-add-content-modal  ul.tabs li{
float:right;
list-style: none;
z-index: 44;
}


.tab a {
  background: #eee; 
  padding: 10px;
  padding-left:20px;
  padding-right:20px; 
  border: 1px solid #ccc; 
  margin-left: -10px; 
    position: relative;
  left: 10px; 
}
.tab a.active {
  background: #FEFEFE; 
  border-bottom:1px solid #FEFEFE; 
}
</style>
<div class="destinations-add-content-modal">
  <ul class="tabs">
    <?php foreach($groups_array as $group_link):?>
    <li class="tab"><?php print $group_link; ?></li>
    <?php endforeach;?>
  </ul>
  
  
  
  <div class="panels-add-content-modal">
  
    <div class="panels-section-column panels-section-column-categories">
      <div class="inside">
        <div class="panels-categories-box">
        <?php foreach ($categories_array as $category): ?>
          <?php print $category; ?>
        <?php endforeach; ?>
        </div>
        <?php print $root_content; ?>
      </div>
    </div>
  
    <?php print $messages; ?>
  
    <?php if (!empty($header)): ?>
      <div class="panels-categories-description">
        <?php print $header; ?>
      </div>
    <?php endif; ?>
  
    <?php if (!empty($columns)): ?>
    <div class="panels-section-columns">
      <?php foreach ($columns as $column_id => $column): ?>
        <div class="panels-section-column panels-section-column-<?php print $column_id; ?> ">
          <div class="inside">
            <?php print $column; ?>
          </div>
        </div>
      <?php endforeach; ?>
    </div>
    <?php endif; ?>
  </div>
</div>