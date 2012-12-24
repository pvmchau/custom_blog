<?php

function custom_blog_followers(){
  
  $nid = arg(1);
  if(empty($nid)){
    return null;
  }
  
  $sql  = 'SELECT uid FROM {flag_content} WHERE content_id = '.$nid.' ORDER BY fcid DESC ';
  $result = pager_query($sql, 4, 0, NULL);
  $member = _custom_blog_count_member_followers($nid);
  $output = '';
  $blog_node = node_load($nid);
  
  $output = '<div class="invite_blogger_title">Manage Blog: '.$blog_node->title.'</div>';
  $output .= _custom_blog_link('followers',$nid);
  $output .= '<div class="blog_followers">';
  $output .= '<div class="blog_followers_title">Followers</div>';
  $output .= '<div class="blog_followers_sub_title">'.$member.' Members following</div>';
  $output .= '<div class="blog_followers_content">';
  $i = 0;
  while($row = db_fetch_object($result)){
    
    // Load Content 
    $node_profile_contents = content_profile_load('basic', $row->uid, $lang = '', $reset = null);
    // Get taxonomy in user array 
    $discipline = '';
    $geo = '';
    
    foreach($node_profile_contents->taxonomy as $disc){
      if($disc->vid == 2){
        $termpath = url('taxonomy/term/'.$disc->tid);
        $discipline .= '<a href="'.$termpath.'" >: '.$disc->name.'</a>';
      }
      if($disc->vid == 7){
        $termpath = url('taxonomy/term/'.$disc->tid);
        $geo .= '<a href="'.$termpath.'" >: '.$disc->name.'</a>';
      }
    }
    
    if(!empty($discipline)){
       $dics = 'Discipline(s)'.$discipline;
    }
    if(!empty($geo)){
       $geo = 'Geographical Focus '.$geo;
    }
    //Get images user 
    $images = _custom_blog_blogger_bio_get_pic($row->uid,'blogger_bio_followers');
    //Get Name user 
    $name = _custom_blog_blogger_bio_get_user_name($row->uid);
    // Get University
    $university .= $node_profile_contents->field_current_institution[0]['value'];
    
    if($i%2 == 0){
      $class = 'one';
    }else{
      $class = 'two';
    }
    $output .= '<div class="blog_followers_items '.$class.'" >';
    $output .= '<div class="followers_user_images" >'.$images.'</div>';
    
    $output .= '<div class="followers_user_body" >';
    $output .= '<div class="followers_user_name" >'.$name.'</div>';
    $output .= '<div class="followers_user_univer" >'.$university.'</div>';
    $output .= '<div class="followers_user_disc" >'.$dics.'</div>';
    $output .= '<div class="followers_user_geo" >'.$geo.'</div>';
    $output .= '</div>';
    $output .= '<div class="clearfix"></div>';
    $output .='</div>';
    $i++ ;
  }
  $output .= theme('pager',NULL,4,0);
  $output .= '</div>';
  $output .= '</div>';
  
  return $output;
  
}