<?php
/**
 * Default theme for prev next element
 */
?>
<ul class="dpi-prev-next">
    <li class="dpi-prev">
        <?php if(isset($variables["prev"]) && !empty($variables["prev"])){ ?>
          <a id="dpi-prev-a" href="<?php echo (isset($variables["prev"]["alias"]) && $variables["prev"]["alias"]!="") ? $variables["prev"]["alias"] : $variables["prev"]["link"]; ?>">
            <span class="dpi-glyph-prev"></span>
            <span class="dpi-prev-link"><?php echo $variables["prev"]["title"] ?></span>
          </a>        
        <?php } ?>
    </li>
    <li class="dpi-next">
        <?php if(isset($variables["next"]) && !empty($variables["next"])){ ?>
          <a id="dpi-next-a" href="<?php echo (isset($variables["next"]["alias"]) && $variables["next"]["alias"]!="") ? $variables["next"]["alias"] : $variables["next"]["link"]; ?>">
              <span class="dpi-glyph-next"></span>
              <span class="dpi-next-link"><?php echo $variables["next"]["title"] ?></span>
          </a>        
        <?php } ?>
    </li>
</ul>