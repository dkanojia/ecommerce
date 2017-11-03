<?php

/**
 * @file
 * Inc File for get category detail in api.
 */

function _get_featured_pic_api_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_featured_pic_api_jsondata(),
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

function get_featured_pic_api_jsondata() {
  
  $query = db_select('node','n');
  
  $query->leftjoin('field_data_field_wears','wears','wears.entity_id=n.nid');
  $query->leftjoin('field_data_field_product_code','prd_code','prd_code.entity_id=n.nid');
  
  $query->leftjoin('field_data_field_market_status','mkt_sts','mkt_sts.entity_id=n.nid');
  $query->leftjoin('taxonomy_term_data','tax','tax.tid=mkt_sts.field_market_status_tid');
  
  $query->leftjoin('field_data_field_description','descr','descr.entity_id=n.nid');
  
  $query->leftjoin('field_data_field_category_id','categ','categ.entity_id=n.nid');
  $query->leftjoin('taxonomy_term_data','cat_tax','cat_tax.tid=categ.field_category_id_tid');
  
  $query->leftjoin('field_data_field_display_image','dis_img','dis_img.entity_id=n.nid');
  $query->leftjoin('file_managed','f_m','f_m.fid=dis_img.field_display_image_fid');
  
  $query->fields('n',array('nid')); 
  $query->fields('wears',array('field_wears_value')); 
  $query->fields('n',array('title')); 
  $query->fields('prd_code',array('field_product_code_value')); 
  $query->fields('tax',array('name')); 
  $query->fields('descr',array('field_description_value')); 
  $query->fields('cat_tax',array('tid','name')); 
  $query->fields('f_m',array('uri')); 

  $query->condition('n.type','featured_picks');
  $result = $query->execute()->fetchAll();

  $res_conv_arr = cvf_convert_object_to_array($result);

  $res_conv_arr = array_map(function($tag) {
    return array(
        'id' => $tag['nid'],
        'wears' => $tag['field_wears_value'],
        'display_name' => $tag['title'],
        'product_code' => $tag['field_product_code_value'],
        'market_status' => $tag['name'],
        'description' => $tag['field_description_value'],
        'category_id' => $tag['tid'],
        'category_name' => $tag['cat_tax_name'],
        'display_image' => $tag['uri'],
    ); }, $res_conv_arr);

    foreach($res_conv_arr as $key => $value){
      
      if($value['display_image'] == 1)
        $res_conv_arr[$key]['wears'] = 'F';
      else
        $res_conv_arr[$key]['wears'] = 'M';
      
      $res_conv_arr[$key]['display_image'] = file_create_url($value['display_image']);
    }

  if(!empty($res_conv_arr)){
    $json_output['status'] = "success";
    $json_output['code'] = 200;
    $json_output['data'] = $res_conv_arr;
  }else{
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Featured picks details empty";
  }

  drupal_json_output($json_output);  
  drupal_exit();
}