<?php

/**
 * @file
 * Inc File for get product detail in api.
 */

// PHP $_SERVER['HTTP_HOST']->request->uri();

function _store_product_inquiry_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => store_product_inquiry_jsondata(),
  );
  return $elements;
}

function _store_general_inquiry_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => store_general_inquiry_jsondata(),
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

function store_product_inquiry_jsondata(){
  $received = file_get_contents("php://input"); // Use inputstream module.
  $received = json_decode($received, TRUE);
  global $user;

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
    
    if(array_key_exists('person_name', $received)){
      $new_arr['person_name'] = $received['person_name'];
    }else{
      $json_output['message'] = "person_name key is not available";
      drupal_json_output($json_output);  
      drupal_exit();  
    }
    
    if(array_key_exists('email_id', $received)){
      $new_arr['email_id'] = $received['email_id'];
    }else{
      $json_output['message'] = "email_id key is not available";
      drupal_json_output($json_output);  
      drupal_exit();
    }

    if(array_key_exists('contact_no', $received)){
      $new_arr['contact_no'] = $received['contact_no'];
    }else{
      $json_output['message'] = "contact_no key is not available";
      drupal_json_output($json_output);  
      drupal_exit();
    }

    if(array_key_exists('description', $received)){
      $new_arr['description'] = $received['description'];
    }else{
      $json_output['message'] = "description key is not available";
      drupal_json_output($json_output);  
      drupal_exit();
    }

    if(array_key_exists('is_product_inquiry', $received)){
      $new_arr['is_product_inquiry'] = $received['is_product_inquiry'];
    }else{
      $json_output['message'] = "is_product_inquiry key is not available";
      drupal_json_output($json_output);  
      drupal_exit();
    }
  }

  if(!empty($new_arr)){
    // add node properties
    $newNode = (object) NULL;
    $newNode->type = 'inquiry_form';
    $newNode->title = 'Product Enquiry';
    $newNode->body['und']['0']['value'] = $new_arr['description'];
    $newNode->field_person_name['und']['0']['value'] = $new_arr['person_name'];
    $newNode->field_email_id['und']['0']['value'] = $new_arr['email_id'];
    $newNode->field_contact_no['und']['0']['value'] = $new_arr['contact_no'];
    $newNode->field_is_product_inquiry['und']['0']['value'] = $new_arr['is_product_inquiry'];

    $newNode->uid = 1;
    $newNode->created = strtotime("now");
    $newNode->status = 1;
    $newNode->language = LANGUAGE_NONE;
    node_save($newNode);

    if(!empty($newNode)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = $newNode->nid;
      $json_output['message'] = "Product enquiry successfully added";
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

function store_general_inquiry_jsondata(){
  $received = file_get_contents("php://input"); // Use inputstream module.
  $received = json_decode($received, TRUE);
  global $user;

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
    
    if(array_key_exists('person_name', $received)){
      $new_arr['person_name'] = $received['person_name'];
    }else{
      $json_output['message'] = "person_name key is not available";
      drupal_json_output($json_output);  
      drupal_exit();  
    }
    
    if(array_key_exists('email_id', $received)){
      $new_arr['email_id'] = $received['email_id'];
    }else{
      $json_output['message'] = "email_id key is not available";
      drupal_json_output($json_output);  
      drupal_exit();
    }

    if(array_key_exists('contact_no', $received)){
      $new_arr['contact_no'] = $received['contact_no'];
    }else{
      $json_output['message'] = "contact_no key is not available";
      drupal_json_output($json_output);  
      drupal_exit();
    }

    if(array_key_exists('description', $received)){
      $new_arr['description'] = $received['description'];
    }else{
      $json_output['message'] = "description key is not available";
      drupal_json_output($json_output);  
      drupal_exit();
    }
  }

  if(!empty($new_arr)){
    // add node properties
    $newNode = (object) NULL;
    $newNode->type = 'inquiry_form';
    $newNode->title = 'General Enquiry';
    $newNode->body['und']['0']['value'] = $new_arr['description'];
    $newNode->field_person_name['und']['0']['value'] = $new_arr['person_name'];
    $newNode->field_email_id['und']['0']['value'] = $new_arr['email_id'];
    $newNode->field_contact_no['und']['0']['value'] = $new_arr['contact_no'];
    $newNode->field_is_product_inquiry['und']['0']['value'] = 0;

    $newNode->uid = 1;
    $newNode->created = strtotime("now");
    $newNode->status = 1;
    $newNode->language = LANGUAGE_NONE;
    node_save($newNode);

    if(!empty($newNode)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = $newNode->nid;
      $json_output['message'] = "Enquiry successfully added";
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