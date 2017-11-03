<?php

/**
 * @file
 * Inc File for get product detail in api.
 */

// PHP $_SERVER['HTTP_HOST']->request->uri();

function _store_subscribe_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => store_subscribe_jsondata(),
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

function store_subscribe_jsondata(){
  $received = file_get_contents("php://input"); // Use inputstream module.
  $received = json_decode($received, TRUE);
  global $user;

  if(empty($received)){
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Please send some data";
    drupal_json_output($json_output);  
    drupal_exit();
  }else{
    $new_arr = [];
    if(array_key_exists('email', $received)){
      $new_arr['email'] = $received['email'];
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "email key doesn't exist in the json";
      drupal_json_output($json_output);  
      drupal_exit();
    }
  }

  if(!empty($new_arr)){
    // add node properties
    $newNode = (object) NULL;
    $newNode->type = 'subscibe_us';
    $newNode->title = 'Anonymous';
    $newNode->field_email['und']['0']['value'] = $new_arr['email'];

    $newNode->uid = 1;
    $newNode->created = strtotime("now");
    $newNode->status = 1;
    $newNode->language = LANGUAGE_NONE;
    node_save($newNode);

    if(!empty($newNode)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = $newNode->nid;
      $json_output['message'] = "You have successfully subscribe us";
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "Sorry you have not subscribe us";
    }
    drupal_json_output($json_output);  
    drupal_exit();

  }else{
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Sorry mail is empty or key is wrong";
    drupal_json_output($json_output);  
    drupal_exit();
  }
}