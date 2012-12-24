<?php

function custom_blog_invite_friends($form_state) {
  
  $form = array();

  $form = custom_blog_invite_friends_nodes();
  return $form;
  
}
function custom_blog_invite_friends_nodes(){
  
  $form['email'] = array(
    '#title' => 'Email',
    '#type' => 'textfield',
    '#description' => 'Enter email addresses or usernames seperated by commas - You enter up to 10 at a time ',
    '#required' => TRUE
  );
  $form['message'] = array(
    '#title' => 'Message',
    '#type' => 'textarea',
    '#cols' => 20,
    '#description' => 'This message will be added to the mail sent to the person(s) you are inviting. ',
    '#required' => TRUE
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Post'),
    '#submit' => array('custom_blog_invite_friends_submit'),
  );
  $form['#theme'] = 'custom_blog_invite_friends_nodes';
  return $form;
}
function theme_custom_blog_invite_friends_nodes($form){
  $nid = arg(1);
  $blog_node = node_load($nid);
  
  $output = '<div class="invite_blogger_title">Manage Blog: '.$blog_node->title.'</div>';
  $output .= _custom_blog_link('invite friends',$nid);
  $output .= '<div class="blog_blogger_content">';
  
  $output .= '<div class="title_invite_friends">
                  <span class="blog_post_header_title">Invite</span>
                  your friends to follow '. $blog_node->title .'
              </div>';
  $output .= drupal_render($form['email']); 
  $output .= drupal_render($form['message']);              
  $output .= drupal_render($form['submit']);              
  $output .= '</div>';
  $output .= drupal_render($form);
  
  return $output;
  
}
function custom_blog_invite_friends_submit($form_state){
  $nid = arg(1);
  $user = _custom_blog_get_user($nid);
  $name = _custom_blog_blogger_bio_get_user_name($user->uid,'name');
  $blog_node = node_load($nid);
  $body = $form_state['#post']['message'];
  $email_to = $form_state['#post']['email'];
  
  //$body = '<b style="font-size:18px;">Dear Colleague,<br /><br />';
//  $body .='I am the founder of the blog named '.$blog_node->title.' on Academic Room.  I invite you to join this forum as a co-blogger.<br /><br />';
//  $body .='Blog Description: '.$blog_node->field_blog_description[0]['value'].'<br /><br />';
//  $body .='To join the blog as a blogger, click on http://staging.academicroom.com/og/subscribe/'.$nid.'<br /><br />';
//  $body .='Best wishes,<br /><br />';
//  $body .= $mess_body.'<br /><br />';
//  $body .= $name.'</b>';

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
              <span style="font-size:13px;font-family: Arial,"Helvetica Neue",Helvetica,sans-serif;">'. nl2br($body) .'</span>
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
    'subject' => 'Followers',
    'body' => $final_body_email,
    'headers' => array(
      'From' => $default_from ,
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/html;charset=utf-8'),
    );
  drupal_mail_send($message);
  drupal_set_message('Email sent successfully');
}