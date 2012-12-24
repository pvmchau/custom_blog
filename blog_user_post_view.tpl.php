<?php
dpm($account);
?>
<style>
#blogs_users_post .views-field-field-blog-post-text-value {
  
}
#blogs_users_post .views-field-field-blog-post-text-value img {
  width: 260px;
  height: 190px;
  float: left;
  margin-right: 15px;
  margin-bottom: 15px;
}
#blogs_users_post .blogger_warpper {
   border-top: 4px solid #87ADDC;
    padding: 20px 10px;
    border-bottom: 1px solid #DEDEDE;
}

</style>
<div class="blog" id="blogs_users_post">
<div class="blogger_warpper">
<div class="blogger_bio_content" style="margin-left:20px;">
<div class="blogger_images"><img class="imagecache imagecache-blogger_bio_image" title="" alt="" 
src="http://staging.academicroom.com/sites/default/files/imagecache/blogger_bio_image//sites/default/files/thumbbig.gif" />
</div>
<div class="blogger_body">
    <div class="blogger_name"><a href=""><?php print $account_profile->title ?></a></div>
    <div class="blogger_iam"></div>
    <div class="blogger_univer"></div>
</div>
<div class="clearfix"></div>
<div class="blog_blogger_bio_view_profile"><a href="/users/<?php print $account->name ?>"><span>View Profile</span></a></div>
</div>
</div>



 <div class="view-og-blog-profile ">
<?php foreach($blog as $node): ?>
<div class="views-row views-row-2 views-row-even" style="clear: both;">
      
  <div class="views-field-created"></div>
  
  <div class="views-field-title">
                <span class="field-content"><a href="/<?php print $node->path ?>"><h3><?php print $node->title ?></h3></a></span>
  </div>
  
  <div class="views-field-name">
                <span class="field-content"><a href="/users/<?php print $account->name ?>"><?php print $account_profile->title ?></a></span>
  </div>
  
  <div class="views-field-field-blog-post-text-value">
                <div class="field-content">
              
                <?php  //print $node->des; 
                
                ?>
                
              
                </div>
  </div>
  <div class="views-field-comment-count"></div>
  <div class="views-field-phpcode"></div>
</div>
<?php endforeach; ?>
</div> 
</div>
