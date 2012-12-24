<?php

function custom_blog_posts($form_state) {
  $form = array();
  
  //Node link unpublish
  $type_link = arg(3);
  if(isset($type_link) && $type_link =='unpublish'){
    $id_child = arg(4);
    _custom_blog_node_unpublish_link($id_child);
  }
  //
  $form['admin']  = custom_blog_admin_nodes();

  return $form;
}


/**
 * Form builder: Builds the node administration overview.
 */
function custom_blog_admin_nodes() {
  $nid = arg(1);
  $result = db_query('
  SELECT n.*, u.name FROM {node} n  INNER JOIN {og_ancestry} an ON n.nid = an.nid INNER JOIN {users} u ON n.uid = u.uid  WHERE n.type = "blog_post"  AND an.group_nid = '.$nid.' AND n.status = 1 ORDER BY n.changed DESC');
  
  $form['options']['submit_unpublish'] = array(
    '#type' => 'submit',
    '#value' => t('Unpublish'),
    '#submit' => array('custom_blog_node_unpublish'),
  );
  $form['options']['submit_del'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('custom_blog_node_delete'),
  );
  

  $languages = language_list();
  $destination = drupal_get_destination();
  $nodes = array();
  while ($node = db_fetch_object($result)) {
    $nodes[$node->nid] = '';
    $options = empty($node->language) ? array() : array('language' => $languages[$node->language]);
    $form['title'][$node->nid] = array('#value' => l($node->title, 'node/'. $node->nid, $options) .' '. theme('mark', node_mark($node->nid, $node->changed)));
    $date_post = format_date($node->created,'custom','M d, Y');
    $form['date'][$node->nid] = array('#value' => $date_post );

    $form['username'][$node->nid] = array('#value' => theme('username', $node));
    
   
    $form['operations'][$node->nid] = array('#value' => l(t('Unpublish'), 'blogs/'.$nid.'/posts/unpublish/'.$node->nid.'').' '.l(t('Edit'), 'node/'. $node->nid .'/edit', array('query' => $destination)).' '.l(t('Delete'), 'node/'. $node->nid .'/delete', array('query' => $destination)));
  }
  $form['nodes'] = array('#type' => 'checkboxes', '#options' => $nodes);
  //$form['pager'] = array('#value' => theme('pager', NULL, 10, 0));
  $form['#theme'] = 'custom_blog_admin_nodes';
  return $form;
}


/**
 * Theme node administration overview.
 *
 * @ingroup themeable
 */
function theme_custom_blog_admin_nodes($form) {
  // If there are rows in this form, then $form['title'] contains a list of
  // the title form elements.
  $nid = arg(1);
  $output = '';
  $blog_node = node_load($nid);
  $output .= _custom_blog_manage_title($blog_node->title);
  $output .= _custom_blog_link('posts',$nid);
  $output .= '<div class="wrapper_blogger" >';
  $output .= '<div class="blog_post_header_title" >Post</div>';
  $output .= '<div class="blog_post_sub_header_title">Select the posts you want to Publish or Delete</div>';
  
  $has_posts = isset($form['title']) && is_array($form['title']);
  $select_header = $has_posts ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Title'), 'Date', 'Blogger', 'Operations');

  $output .= drupal_render($form['options']);
  if ($has_posts) {
    foreach (element_children($form['title']) as $key) {
      $row = array();
      $row[] = drupal_render($form['nodes'][$key]);
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['date'][$key]);
      
      $row[] = drupal_render($form['username'][$key]);
        
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
    }

  }
  else {
    $rows[] = array(array('data' => t('The blog currently has no posts. <a href="/node/add/blog-post/'.$nid.'">Create a new post</a>.'), 'colspan' => '5'));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);
  $output .= '</div>';
  return $output;
}

function custom_blog_node_delete($form_state){
    $nid = arg(1);
    $nodes = $form_state['#post']['nodes'];
    if(!empty($nodes)){
      foreach ($nodes as $nid) {
        node_delete($nid);
      }
      $form_state['redirect'] = 'blogs/'.$nid.'/posts';
    }else{
      drupal_set_message('Please select blogs posts','warning');
    }
    return;
}
function custom_blog_node_unpublish($form_state){
    $nid = arg(1);
    $nodes = $form_state['#post']['nodes'];
    $title = '';
    if(!empty($nodes)){
      foreach ($nodes as $nid) {
        $node = node_load($nid);
        $node->status = 0;
        $title .= $node->title." ";
        node_save($node);
      }
      drupal_set_message('Blog post '.$title.' unpublish');
      $form_state['redirect'] = 'blogs/'.$nid.'/posts';
    }else{
      drupal_set_message('Please select blogs posts','warning');
    }
    return;
}

function _custom_blog_node_unpublish_link($nid){
  if(empty($nid)){
    drupal_set_message('Node id null','warning');
  }
  $node = node_load($nid);
  $node->status = 0;
  node_save($node);
  drupal_set_message('Blog post '.$node->title.' unpublish');
  drupal_goto('blogs/'.arg(1).'/posts');
}
