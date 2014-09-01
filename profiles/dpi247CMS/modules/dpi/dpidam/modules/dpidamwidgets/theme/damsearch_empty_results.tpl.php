<div class="dam-search-results">
  <?php if (isset($messages) && is_array($messages)) : ?>
    <?php foreach ($messages as $message): ?>
    <h2><?php print $message ?></h2>
    <?php endforeach; ?>
  <?php endif; ?>
</div>