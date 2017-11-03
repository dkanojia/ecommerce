<?php

/**
 * @file
 * Inc File for get product detail in api.
 */

// PHP $_SERVER['HTTP_HOST']->request->uri();

function _get_mens_prd_api_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_mens_prd_api_jsondata(),
  );
  return $elements;
}

function _get_womens_prd_api_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_womens_prd_api_jsondata(),
  );
  return $elements;
}

function _get_prd_list_api_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_prd_list_api_jsondata(),
  );
  return $elements;
}

function _update_total_views() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => update_total_views_jsondata(),
  );
  return $elements;
}


function cvf_convert_object_to_array($data) {

  if (is_object($data)) {
      $data = get_object_vars($data);
  }

  if (is_array($data)) {
      return array_map(__FUNCTION__, $data);
  }
  else {
      return $data;
  }
}

function get_mens_prd_api_jsondata() {

  $received = file_get_contents("php://input"); // Use inputstream module.
  $received = json_decode($received, TRUE);

  if(empty($received)){
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Please send type of category";
    drupal_json_output($json_output);  
    drupal_exit();
  }else{
    if(array_key_exists('type', $received)){
      $type = $received['type'];
      
      if($received['type'] !== 'M'){
        $json_output['status'] = "error";
        $json_output['code'] = 400;
        $json_output['data'] = "Please send type of category as M";
        drupal_json_output($json_output);  
        drupal_exit();
      }

    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "key : type doesn't exist in the json";
      drupal_json_output($json_output);  
      drupal_exit();
    }
  }
 
  if($type == 'M'){

    $id = 1;
    
    $cat_id_arr = [];
    $cat_arr = taxonomy_get_children($id);
    $cat_id_arr [] = $id;
    foreach ($cat_arr as $child_cat) {
      $cat_id_arr [] = $child_cat->tid;
      $child_cat_arr = taxonomy_get_children($child_cat->tid);

      foreach ($child_cat_arr as $sub_child_cat) {
        $cat_id_arr [] = $sub_child_cat->tid;
      }
    }

    $query = db_select('node','n');
    
    $query->leftjoin('uc_products','uc_prd','uc_prd.nid=n.nid');
    $query->leftjoin('field_data_field_short_description','sht_decr','sht_decr.entity_id=n.nid');
    $query->leftjoin('field_data_body','decr','decr.entity_id=n.nid');
    
    $query->leftjoin('field_data_field_material_type','mat_type','mat_type.entity_id=n.nid');
    $query->leftjoin('taxonomy_term_data','tax','tax.tid=mat_type.field_material_type_tid');

    $query->leftjoin('field_data_uc_product_image','uc_im','uc_im.entity_id=n.nid');
    $query->leftjoin('file_managed','f_m','f_m.fid=uc_im.uc_product_image_fid');

    $query->leftjoin('field_data_field_hd_image','hd_im','hd_im.entity_id=n.nid');
    $query->leftjoin('file_managed','fl_m','fl_m.fid=hd_im.field_hd_image_fid');
    
    $query->leftjoin('field_data_taxonomy_catalog','t_ctlg','t_ctlg.entity_id=n.nid');
    $query->leftjoin('taxonomy_term_data','tax_ctlg','tax_ctlg.tid=t_ctlg.taxonomy_catalog_tid');
    
    $query->leftjoin('taxonomy_term_hierarchy','tax_hrchy','tax_hrchy.tid=tax_ctlg.tid');
    $query->leftjoin('taxonomy_term_data','parent_name','parent_name.tid=tax_hrchy.parent');

    $query->fields('n',array('nid','title')); 
    $query->fields('sht_decr',array('field_short_description_value')); 
    $query->fields('decr',array('body_value')); 
    $query->fields('tax',array('tid','name')); 
    $query->fields('tax_ctlg',array('tid','name')); 
    $query->fields('tax_hrchy',array('parent')); 
    $query->fields('parent_name',array('name')); 
    $query->fields('f_m',array('uri'));
    $query->fields('fl_m',array('uri'));
    $query->fields('uc_prd',array('model','sell_price','cost','list_price')); 

    $query->condition('t_ctlg.taxonomy_catalog_tid ',$cat_id_arr,'IN');
    $query->condition('n.type','product');
    $result = $query->execute()->fetchAll();

    // $file_url = file_create_url($res_conv_arr['0']['uri']);
    $res_conv_arr = cvf_convert_object_to_array($result);

    $res_conv_arr = array_map(function($tag) {
      return array(
          'id' => $tag['nid'],
          'display_code' => $tag['model'],
          'name' => $tag['title'],
          'short_description' => $tag['field_short_description_value'],
          'long_description' => $tag['body_value'],
          'material_type_id' => $tag['tid'],
          'material_type' => $tag['name'],
          'cateory_id' => $tag['tax_ctlg_tid'],
          'category_name' => $tag['tax_ctlg_name'],
          'parent_category_id' => $tag['parent'],
          'parent_category_name' => $tag['parent_name_name'],
          'display_image' => $tag['uri'],
          'hd_image' => $tag['fl_m_uri'],
          'sell_price' => $tag['sell_price'],
          'cost' => $tag['cost'],
          'list_price' => $tag['list_price'],
      ); }, $res_conv_arr);

      foreach($res_conv_arr as $key => $value){
        $res_conv_arr[$key]['display_image'] = file_create_url($value['display_image']);
        $res_conv_arr[$key]['hd_image'] = file_create_url($value['hd_image']);
      }

    if(!empty($res_conv_arr)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = $res_conv_arr;
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "Sorry no data";
    }
    drupal_json_output($json_output);  
    drupal_exit();
  }
}

function get_womens_prd_api_jsondata() {

  $received = file_get_contents("php://input"); // Use inputstream module.
  $received = json_decode($received, TRUE);

  if(empty($received)){
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Please send type of category";
    drupal_json_output($json_output);  
    drupal_exit();
  }else{
    if(array_key_exists('type', $received)){
      $type = $received['type'];
      
      if($received['type'] !== 'F'){
        $json_output['status'] = "error";
        $json_output['code'] = 400;
        $json_output['data'] = "Please send type of category as F";
        drupal_json_output($json_output);  
        drupal_exit();
      }

    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "key : type doesn't exist in the json";
      drupal_json_output($json_output);  
      drupal_exit();
    }
  }
 
  if($type == 'F'){

    $id = 2;
    
    $cat_id_arr = [];
    $cat_arr = taxonomy_get_children($id);
    $cat_id_arr [] = $id;
    foreach ($cat_arr as $child_cat) {
      $cat_id_arr [] = $child_cat->tid;
      $child_cat_arr = taxonomy_get_children($child_cat->tid);

      foreach ($child_cat_arr as $sub_child_cat) {
        $cat_id_arr [] = $sub_child_cat->tid;
      }
    }

    $query = db_select('node','n');
    
    $query->leftjoin('uc_products','uc_prd','uc_prd.nid=n.nid');
    $query->leftjoin('field_data_field_short_description','sht_decr','sht_decr.entity_id=n.nid');
    $query->leftjoin('field_data_body','decr','decr.entity_id=n.nid');
    
    $query->leftjoin('field_data_field_material_type','mat_type','mat_type.entity_id=n.nid');
    $query->leftjoin('taxonomy_term_data','tax','tax.tid=mat_type.field_material_type_tid');

    $query->leftjoin('field_data_uc_product_image','uc_im','uc_im.entity_id=n.nid');
    $query->leftjoin('file_managed','f_m','f_m.fid=uc_im.uc_product_image_fid');

    $query->leftjoin('field_data_field_hd_image','hd_im','hd_im.entity_id=n.nid');
    $query->leftjoin('file_managed','fl_m','fl_m.fid=hd_im.field_hd_image_fid');
    
    $query->leftjoin('field_data_taxonomy_catalog','t_ctlg','t_ctlg.entity_id=n.nid');
    $query->leftjoin('taxonomy_term_data','tax_ctlg','tax_ctlg.tid=t_ctlg.taxonomy_catalog_tid');
    
    $query->leftjoin('taxonomy_term_hierarchy','tax_hrchy','tax_hrchy.tid=tax_ctlg.tid');
    $query->leftjoin('taxonomy_term_data','parent_name','parent_name.tid=tax_hrchy.parent');

    $query->fields('n',array('nid','title')); 
    $query->fields('sht_decr',array('field_short_description_value')); 
    $query->fields('decr',array('body_value')); 
    $query->fields('tax',array('tid','name')); 
    $query->fields('tax_ctlg',array('tid','name')); 
    $query->fields('tax_hrchy',array('parent')); 
    $query->fields('parent_name',array('name')); 
    $query->fields('f_m',array('uri'));
    $query->fields('fl_m',array('uri'));
    $query->fields('uc_prd',array('model','sell_price','cost','list_price')); 

    $query->condition('t_ctlg.taxonomy_catalog_tid ',$cat_id_arr,'IN');
    $query->condition('n.type','product');
    $result = $query->execute()->fetchAll();

    // $file_url = file_create_url($res_conv_arr['0']['uri']);
    $res_conv_arr = cvf_convert_object_to_array($result);

    $res_conv_arr = array_map(function($tag) {
      return array(
          'id' => $tag['nid'],
          'display_code' => $tag['model'],
          'name' => $tag['title'],
          'short_description' => $tag['field_short_description_value'],
          'long_description' => $tag['body_value'],
          'material_type_id' => $tag['tid'],
          'material_type' => $tag['name'],
          'cateory_id' => $tag['tax_ctlg_tid'],
          'category_name' => $tag['tax_ctlg_name'],
          'parent_category_id' => $tag['parent'],
          'parent_category_name' => $tag['parent_name_name'],
          'display_image' => $tag['uri'],
          'hd_image' => $tag['fl_m_uri'],
          'sell_price' => $tag['sell_price'],
          'cost' => $tag['cost'],
          'list_price' => $tag['list_price'],
      ); }, $res_conv_arr);

      foreach($res_conv_arr as $key => $value){
        $res_conv_arr[$key]['display_image'] = file_create_url($value['display_image']);
        $res_conv_arr[$key]['hd_image'] = file_create_url($value['hd_image']);
      }

    if(!empty($res_conv_arr)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = $res_conv_arr;
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "Sorry no data";
    }
    
    drupal_json_output($json_output);  
    drupal_exit();
  }
}

function get_prd_list_api_jsondata() {

  $received = file_get_contents("php://input"); // Use inputstream module.
  $received = json_decode($received, TRUE);

  if(empty($received)){
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Please send category id";
    drupal_json_output($json_output);  
    drupal_exit();
  }else{
    if(array_key_exists('category_id', $received)){
      $id = $received['category_id'];
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "category_id key doesn't exist in the json";
      drupal_json_output($json_output);  
      drupal_exit();
    }
  }
  
  $cat_id_arr = [];
  $cat_arr = taxonomy_get_children($id);
  $cat_id_arr [] = $id;
  foreach ($cat_arr as $child_cat) {
    $cat_id_arr [] = $child_cat->tid;
    $child_cat_arr = taxonomy_get_children($child_cat->tid);

    foreach ($child_cat_arr as $sub_child_cat) {
      $cat_id_arr [] = $sub_child_cat->tid;
    }
  }

  $query = db_select('node','n');
  
  $query->leftjoin('uc_products','uc_prd','uc_prd.nid=n.nid');
  $query->leftjoin('field_data_field_short_description','sht_decr','sht_decr.entity_id=n.nid');
  $query->leftjoin('field_data_body','decr','decr.entity_id=n.nid');
  
  $query->leftjoin('field_data_field_material_type','mat_type','mat_type.entity_id=n.nid');
  $query->leftjoin('taxonomy_term_data','tax','tax.tid=mat_type.field_material_type_tid');

  $query->leftjoin('field_data_uc_product_image','uc_im','uc_im.entity_id=n.nid');
  $query->leftjoin('file_managed','f_m','f_m.fid=uc_im.uc_product_image_fid');

  $query->leftjoin('field_data_field_hd_image','hd_im','hd_im.entity_id=n.nid');
  $query->leftjoin('file_managed','fl_m','fl_m.fid=hd_im.field_hd_image_fid');
  
  $query->leftjoin('field_data_taxonomy_catalog','t_ctlg','t_ctlg.entity_id=n.nid');
  $query->leftjoin('taxonomy_term_data','tax_ctlg','tax_ctlg.tid=t_ctlg.taxonomy_catalog_tid');
  
  $query->leftjoin('taxonomy_term_hierarchy','tax_hrchy','tax_hrchy.tid=tax_ctlg.tid');
  $query->leftjoin('taxonomy_term_data','parent_name','parent_name.tid=tax_hrchy.parent');

  $query->fields('n',array('nid','title')); 
  $query->fields('sht_decr',array('field_short_description_value')); 
  $query->fields('decr',array('body_value')); 
  $query->fields('tax',array('tid','name')); 
  $query->fields('tax_ctlg',array('tid','name')); 
  $query->fields('tax_hrchy',array('parent')); 
  $query->fields('parent_name',array('name')); 
  $query->fields('f_m',array('uri'));
  $query->fields('fl_m',array('uri'));
  $query->fields('uc_prd',array('model','sell_price','cost','list_price')); 

  $query->condition('t_ctlg.taxonomy_catalog_tid ',$cat_id_arr,'IN');
  $query->condition('n.type','product');
  $result = $query->execute()->fetchAll();

  // $file_url = file_create_url($res_conv_arr['0']['uri']);
  $res_conv_arr = cvf_convert_object_to_array($result);

  $res_conv_arr = array_map(function($tag) {
    return array(
        'id' => $tag['nid'],
        'display_code' => $tag['model'],
        'name' => $tag['title'],
        'short_description' => $tag['field_short_description_value'],
        'long_description' => $tag['body_value'],
        'material_type_id' => $tag['tid'],
        'material_type' => $tag['name'],
        'cateory_id' => $tag['tax_ctlg_tid'],
        'category_name' => $tag['tax_ctlg_name'],
        'parent_category_id' => $tag['parent'],
        'parent_category_name' => $tag['parent_name_name'],
        'display_image' => $tag['uri'],
        'hd_image' => $tag['fl_m_uri'],
        'sell_price' => $tag['sell_price'],
        'cost' => $tag['cost'],
        'list_price' => $tag['list_price'],
    ); }, $res_conv_arr);

    foreach($res_conv_arr as $key => $value){
      $res_conv_arr[$key]['display_image'] = file_create_url($value['display_image']);
      $res_conv_arr[$key]['hd_image'] = file_create_url($value['hd_image']);
    }

  if(!empty($res_conv_arr)){
    $json_output['status'] = "success";
    $json_output['code'] = 200;
    $json_output['data'] = $res_conv_arr;
  }else{
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Sorry no data";
  }

  drupal_json_output($json_output);  
  drupal_exit();   
}

function update_total_views_jsondata(){

  $received = file_get_contents("php://input"); // Use inputstream module.
  $received = json_decode($received, TRUE);

  if(empty($received)){
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['message'] = "Please send some data";
    drupal_json_output($json_output);  
    drupal_exit();
  }else{
    $new_arr = [];

    $json_output['status'] = "error";
    $json_output['code'] = 400;
    
    if(array_key_exists('node_id', $received)){
      $new_arr['node_id'] = $received['node_id'];
    }else{
      $json_output['message'] = "node_id key is not available";
      drupal_json_output($json_output);  
      drupal_exit();  
    }

    if(array_key_exists('total_views', $received)){
      $new_arr['total_views'] = $received['total_views'];
    }else{
      $json_output['message'] = "total_views key is not available";
      drupal_json_output($json_output);  
      drupal_exit();  
    }
  }

  if(!empty($new_arr)){
    // add node properties
    $node = node_load($new_arr['node_id']);

    $old_views = $node->field_total_view['und']['0']['value'];
    $updated_views = $old_views + $new_arr['total_views'];

    $node->field_total_view['und']['0']['value'] = $updated_views;
    node_save($node);

    if(!empty($node)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = "Product ID : ".$node->nid;
      $json_output['message'] = "Total views ". $updated_views ." updated successfully";
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "Sorry detail not successfully added";
    }
    drupal_json_output($json_output);  
    drupal_exit();

  }else{
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Sorry no data available";
    drupal_json_output($json_output);  
    drupal_exit();
  }

}