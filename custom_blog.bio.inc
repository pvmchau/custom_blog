<?php

function custom_blog_bio($form_state) {
  
  $form = array();
  
  $form = custom_blog_bio_nodes();
  return $form;
  
}
/**
 * Form Blogger Bio
*/
function custom_blog_bio_nodes(){
 
  global $user;
  $nid = arg(1);
  //GET sumary body OLD
  $result = db_query('SELECT summary FROM {blogger_bio} WHERE nid = '.$nid.' AND uid = '.$user->uid.'');
  while($row = db_fetch_object($result)){
      $sumary = $row->summary;
      $sumary = str_replace('<br />
<br />', "\n", $sumary);
  }
  
  $form['sumary'] = array(
    '#type' => 'textarea',
    '#default_value' => $sumary,
    '#cols' => 20,
    '#required' => TRUE,
  );
    
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Post'),
    '#submit' => array('custom_blog_bio_submit'),
  );
  $form['#theme'] = 'custom_blog_bio_nodes';
  return $form;
}

/**
 * Theme Blogger Bio
*/
function theme_custom_blog_bio_nodes($form){
  global $user;
  $nid = arg(1);
  $blogger_nodes = node_load($nid);
  $node_profile_contents = content_profile_load('basic', $user->uid, $lang = '', $reset = null);
  $output = '';
  
    $value_iam = _custom_blog_get_value_select('field_iam',$node_profile_contents->field_iam[0]['value']);
    $output .= '<div class="wrapper_blogger" >';
    $output .= '<div class="blogs_manage_title">Manage Blog:'.$blogger_nodes->title.'</div>';
    $output .= _custom_blog_link('your blog bio',$nid);
    $output .= '<div class="blogger_bio_title" style="margin-left:10px;">Biography that would appear on your blog page</div>';
    $output .= '<div class="blog_blogger_content" style="padding: 5px 10px;">';
    $output .= '<div class="blog_blogger_images" style="float:left;">'._custom_blog_blogger_bio_get_pic($user->uid).'</div>';
    
    $output .= '<div class="blog_blogger_body" >';
    $output .= '<div class="blog_blogger_name" >'._custom_blog_blogger_bio_get_user_name($user->uid).'</div>';
    $output .= '<div class="blog_blogger_iam" >'.$value_iam.'</div>';
    $output .= '<div class="blog_blogger_univer" >'.$node_profile_contents->field_current_institution[0]['value'].'</div>';
    $output .= '<div class="blog_blogger_summary" style="padding-bottom:5px;">'._custom_blog_get_summary(arg(1),$user->uid).'</div>';
    $output .='<div class="clearfix"></div>';
    $output .= '</div>';
    $output .= '</div>';
 
  
  
  /*
  
  $output .= '<div class="wrapper_blogger" >';
  $output .= '<div class="blogger_bio_title" >Biography that appear on your Blog Page</div>';
  
  $output .='<div class="blogger_bio_content" >';
  $output .='<div class="blogger_images" >'._custom_blog_blogger_bio_get_pic($user->uid).'</div>';
  $output .='<div class="blogger_body" >';
  
  $output .='<div class="blog_blogger_name" >'._custom_blog_blogger_bio_get_user_name($user->uid).'</div>';
  $output .='<div class="blogger_iam" >'.$value_iam.'</div>';
  $output .='<div class="blogger_univer" >'.$node_profile_contents->field_current_institution[0]['value'].'</div>';
  //$output .= '<div class="blog_blogger_summary" style="padding-top: 10px;width: 450px;">'._custom_blog_get_summary(arg(1),$user->uid).'</div>';
  
  $output .='</div><div class="clearfix"></div>';
  $output .='</div>';
  */
  
  
  
  
  
  
  
  
  
  
  $output .= '<div class="blogger_bio_form" >';
  $output .= '<div class="blogger_bio_form_title" >
                <div class="title_name">Name</div>
                <div class="title_sumary">Summary</div>
              </div>';
  $output .= '<div class="blogger_bio_form_content" >
                <div class="content_name">';
  $output .= _custom_blog_blogger_bio_get_user_name($user->uid);
  $output .='   </div>
                <div class="content_sumary">';
  $output .= drupal_render($form['sumary']);              
  $output .= drupal_render($form['submit']);              
  $output .=    '</div>
              </div><div class="clearfix"></div></div>';
  
  $output .= drupal_render($form);
  $output .= '</div>';
  return $output;
  
}
/**
 * Form submit 
*/
function custom_blog_bio_submit($form_state){
  global $user;
  $nid = arg(1);
  $sumary = $form_state['#post']['sumary'];
  
  if(empty($nid)){
    drupal_set_message('Sorry id null, please try again ','error');
  }
  $sql = db_query('SELECT nid FROM {blogger_bio} WHERE nid = '.$nid.' AND uid = '.$user->uid.'');
  while($row = db_fetch_object($sql)){
    $check_nid = $row->nid;
  }
  
  if(empty($check_nid)){
    db_query("INSERT INTO {blogger_bio}(nid,summary,uid) VALUES ('".$nid."','".nl2br($sumary)."','".$user->uid."')");
    drupal_set_message(t('Blogger biography was inserted successfully.'), $type = 'status', $repeat = TRUE);
  }else{
    db_query("UPDATE {blogger_bio} SET summary = '".nl2br($sumary)."' WHERE nid = ".$nid." AND uid = ".$user->uid." ");
    drupal_set_message(t('Blogger biography was updated successfully.'), $type = 'status', $repeat = TRUE);
  }
  
  
}

