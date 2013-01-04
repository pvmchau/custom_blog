<?php
function custom_blog_invite_manage_blog($form_state){
  $form = array();
  $form = custom_blog_invite_manage_blog_nodes();
  return $form;
}


function custom_blog_invite_manage_blog_nodes(&$form){
    $nid = arg(1);
    $user = _custom_blog_get_user($nid);
    $name = _custom_blog_blogger_bio_get_user_name($user->uid,'name');
    $blog_node = node_load($nid);
    $body = '
Dear Colleague,

I am the founder of the blog named '.$blog_node->title.' on Academic Room.  I invite you to join this forum as a co-blogger.    

Blog Description: '. $blog_node->field_blog_description[0]['value'] .'
To join the blog as a blogger, click on <a href="http://www.academicroom.com/og/subscribe/'.$nid.'" >http://www.academicroom.com/og/subscribe/'.$nid.'</a>    

';

$body .='Best wishes,
'.$name;

  
  $form['email'] = array(
    '#title' => 'Email',
    '#type' => 'textfield',
    '#description' => 'Enter email addresses or usernames seperated by commas - You enter up to 10 at a time ',
    '#required' => TRUE
  );
  
  $form['message'] = array(
    '#title' => 'Message',
    '#type' => 'textarea',
    '#default_value' => $body, 
    '#description' => 'This message will be added to the mail sent to the person(s) you are inviting. ',
    '#required' => TRUE,
    '#attributes' => array('style' => 'height:350px!important;font-size:13px;font-family: Arial,"Helvetica Neue",Helvetica,sans-serif;'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Post'),
    '#submit' => array('custom_blog_invite_manage_blog_submit'),
  );
  $form['#theme'] = 'custom_blog_invite_manage_blog_nodes';
  return $form;
}

function theme_custom_blog_invite_manage_blog_nodes(&$form){
  if (!is_numeric($nid = arg(1))) {
    if(!is_numeric($nid = arg(3))){
      return '';
    }    
  }
  $node = node_load($nid);
  if($node->type == 'blog') {
    $nid = $node->nid;
    
  }elseif($node->type == 'blog_post'){
      $sql = db_query('SELECT group_nid FROM {og_ancestry} WHERE nid='.$node->nid.'');
      while($row = db_fetch_object($sql)){
        $nid = $row->group_nid; 
      }
  }else{
    $nid= null;
  }
  
  
  $form_theme = drupal_render($form['email']); 
  $form_theme .= drupal_render($form['message']);              
  $form_theme .= drupal_render($form['submit']);              

  //$output .= drupal_render($form);
  $node = node_load($nid);
  //dpm($node);
  drupal_add_js(drupal_get_path('module', 'custom_blog') . '/my_script.js');
  $output = '<div id="custom_invite_manage_blog">';
  $output .= '<div class="blogs_manage_title">Manage Blog: ' . $node->title . '</div>';
  $output .= '<div class="manage_blog_link"><ul class="blog_dashboard">';
  $output .= '<li><a href="#content_invite_blogger">INVITE BLOGGERS</a></li>';
  $output .= '<li><a href="#content_manager_bloger">MANAGER BLOGGERS</a></li>';
  $output .= '</ul></div>';
  $output .= '';
  $output .= '<div id="custom_manager_blog_content"><div class="blog_blogger_content" >';
  $output .= '<div id="content_invite_blogger"><div class="manager_blogger_title">
  <span class="blog_post_header_title">Staff up!</span> Invite friends or other users to write for your blog.</div>
  ' . $form_theme. '
  </div>';
  $output .= '<div id="content_manager_bloger">
  <div class="manager_blogger_title"><span class="blog_post_header_title">Bloggers</span></div>
  ' . custom_blog_list_user_write_blog($nid) . '</div>';
  $output .= '</div></div></div>';
  return $output.drupal_render($form);
}

function custom_blog_list_user_write_blog($nid) {
  $sql = db_query("SELECT * FROM og_uid WHERE nid = %d ORDER BY is_admin DESC, is_active ASC", $nid);
  $items = array();
  while($row = db_fetch_object($sql)) {
    $user = user_load(array('uid' => $row->uid));    
    if($user->status == 1){
      $username_link = url('user/'.$user->uid);
      $username = '<a href="'.$username_link.'" >'.$user->name.'</a>';
      $last_login =  time() - $user->login;
      $login = format_interval($last_login) . ' ago';
      $number_post = db_result(db_query("SELECT COUNT(nid) FROM {node} WHERE type='blog_post' AND uid = %d",$row->uid));
          
      if($row->is_active) {
        if ($row->is_admin) {
          $status = 'Admin';
          $action_admin_role_edit = l(t('Remove Admin'), 'og/delete_admin/'.$nid.'/'.$user->uid, array('query' => drupal_get_destination()));        
        } else {
          $status = 'Blogger';
          $action_admin_role_edit = l(t('Make Admin'), 'og/create_admin/'.$nid.'/'.$user->uid, array('query' => drupal_get_destination()));
        }
        $action_membership_edit = l(t('Delete'), 'og/unsubscribe/'.$nid.'/'.$user->uid, array('query' => drupal_get_destination()));
      } else {
        $status = 'Blogger (approval needed)';
        $action_membership_edit_approve = l(t('approve'), 'og/approve/'.$nid.'/'.$user->uid.'/'.og_get_token($nid), array('query' => drupal_get_destination()));
        $action_membership_edit_deny = l(t('deny'), 'og/deny/'.$nid.'/'.$user->uid.'/'.og_get_token($nid), array('query' => drupal_get_destination()));
        $action_membership_edit = 'Request: '.$action_membership_edit_approve.' or '.$action_membership_edit_deny;
        $action_admin_role_edit = '';
      }
      
      $items[] = array($username, $status, $number_post, $login, $action_membership_edit.'<br>'.$action_admin_role_edit);
    }    
  }
  //dpm($items);
  return theme_table(array('Name', 'Type', 'Posts', 'Last Online', 'Action'), $items);
}

function custom_blog_last_login($seconds, $max_periods) {
    $periods = array("year" => 31536000, "month" => 2419200, "week" => 604800, "day" => 86400, "hour" => 3600, "minute" => 60, "second" => 1);
    $i = 1;
    foreach ( $periods as $period => $period_seconds )
    {
        $period_duration = floor($seconds / $period_seconds);
        $seconds = $seconds % $period_seconds;
        if ( $period_duration == 0 ) continue;
        $duration[] = "{$period_duration} {$period}" . ($period_duration > 1 ? 's' : '');
        $i++;
        if ( $i >  $max_periods ) break;
    }
    return implode(' ', $duration);
}

function custom_blog_invite_manage_blog_submit(&$form_state){
    $nid = arg(1);
    $user = _custom_blog_get_user($nid);
    $name = _custom_blog_blogger_bio_get_user_name($user->uid,'name');
    $blog_node = node_load($nid);
    $body = $form_state['#post']['message'];
    $body =  nl2br($body);
    $email_to = $form_state['#post']['email'];
    
    /*
    $body = '<span style="font-size:13px;font-family: Arial,"Helvetica Neue",Helvetica,sans-serif;">Dear Colleague,<br /><br />';
    $body .='I am the founder of the blog named '.$blog_node->title.' on Academic Room.  I invite you to join this forum as a co-blogger.<br /><br />';
    $body .='Blog Description: '.$blog_node->field_blog_description[0]['value'].'<br /><br />';
    $body .='To join the blog as a blogger, click on <a href="http://www.academicroom.com/og/subscribe/'.$nid.'" >http://www.academicroom.com/og/subscribe/'.$nid.'</a><br /><br />';
    $body .= $mess_body.'<br /><br />';
    $body .='Best wishes,<br />';
    $body .= $name.'</span>';
    */
    
    $final_body_email = '<table cellspacing="0" cellpadding="0" border="0" width="620" style="border-collapse: separate;">
  <tbody>
    <tr>
      <td style="padding: 4pt 6pt; background:rgb(178, 206, 234); border:1px solid #ccc;">
      <b><span style="color: black; font-size: 15px;">Academic Room </span></b>
      </td>
    </tr>
    <tr>
      <td style="padding: 12pt; border:1px solid #ccc; Border-top:none; background:#fff ">
      <table cellspacing="0" cellpadding="0" border="0" width="100%" style="border-collapse: separate;">
          <tbody>
            <tr>
              <td width="583" valign="top" style="font-family: Arial,Helvetica,sans-serif; font-size: 12px;">
              <span style="font-size:13px;font-family: Arial,"Helvetica Neue",Helvetica,sans-serif;">'. $body .'</span>
              </td>
              <td width="10" valign="top">&nbsp;</td>
            </tr>
            </tbody>
        </table>
        </td>
    </tr>
  </tbody>
</table>';
    
    $default_from = variable_get('site_mail', ini_get('sendmail_from'));
    
    $message = array(
      'to' => $email_to,
      'subject' => 'Invitation from '.$name.' to join the '.$blog_node->title.' on Academic Room',
      'body' => $final_body_email,
      'headers' => array(
        'From' => $default_from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/html;charset=utf-8'),
      );
    drupal_mail_send($message);
    drupal_set_message('Email send successfully.');
  
}