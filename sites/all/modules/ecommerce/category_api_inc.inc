<?php

/**
 * @file
 * Inc File for get product detail in api.
 */

function _get_category_api_json() {
  $elements = [];
  
  $elements['api_data'] = array(
    '#markup' => get_category_api_jsondata(),
  );
  return $elements;
}

function _get_filtered_category_api_json() {
  $elements = [];
  
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
      
      if(($type !== 'F') && ($type !== 'M')){
        $json_output['status'] = "error";
        $json_output['code'] = 400;
        $json_output['data'] = "Please send type of category as F or M";
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

  $elements['api_data'] = array(
    '#markup' => get_filtered_category_api_jsondata($type),
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

function get_category_api_jsondata() {
  $cat_id_arr = [];
  $parent_cat_id = 1;
  $taxnomy = taxonomy_term_load($parent_cat_id);
  $vid = intval($taxnomy->vid);

  $tree = taxonomy_get_nested_tree($vid,3);
  $output =output_taxonomy_nested_tree($tree);
  
  if(!empty($output)){
    $json_output['status'] = "success";
    $json_output['code'] = 200;
    $json_output['data'] = $output;
  }else{
    $json_output['status'] = "error";
    $json_output['code'] = 400;
    $json_output['data'] = "Sorry no data";
  }
  drupal_json_output($json_output);  
  drupal_exit();
}

function get_filtered_category_api_jsondata($type) {
  if($type == 'F'){
    $cat_id_arr = [];
    $parent_cat_id = 2;
    $taxnomy = taxonomy_term_load($parent_cat_id);
    $vid = intval($taxnomy->vid);

    $output = taxonomy_get_children($taxnomy->tid);
    
    $arr = array();
    $sec_arr = array();
    foreach ($output as $key => $value) {
      $arr ['id'] = $value->tid;
      $arr ['name'] = $value->name;
      $arr ['description'] = $value->description;
      $sec_arr [] = $arr;
    }

    if(!empty($sec_arr)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = $sec_arr;
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "Sorry no data";
    }
    drupal_json_output($json_output);  
    drupal_exit();
  }

  if($type == 'M'){
    $cat_id_arr = [];
    $parent_cat_id = 1;
    $taxnomy = taxonomy_term_load($parent_cat_id);
    $vid = intval($taxnomy->vid);

    $output = taxonomy_get_children($taxnomy->tid);
    
    $arr = array();
    $sec_arr = array();
    foreach ($output as $key => $value) {
      $arr ['id'] = $value->tid;
      $arr ['name'] = $value->name;
      $arr ['description'] = $value->description;
      $sec_arr [] = $arr;
    }
    
    if(!empty($sec_arr)){
      $json_output['status'] = "success";
      $json_output['code'] = 200;
      $json_output['data'] = $sec_arr;
    }else{
      $json_output['status'] = "error";
      $json_output['code'] = 400;
      $json_output['data'] = "Sorry no data";
    }
    
    drupal_json_output($json_output);  
    drupal_exit();
  }

}

function taxonomy_get_nested_tree($terms = array(), $max_depth = NULL, $parent = 0, $parents_index = array(), $depth = 0) {
  $return = [];
  if (is_int($terms)) {
    $terms = taxonomy_get_tree($terms);
  }

  foreach($terms as $term) {
    foreach($term->parents as $term_parent) {
      if ($term_parent == $parent) {
        $return[$term->tid] = $term;
      }
      else {
        $parents_index[$term_parent][$term->tid] = $term;
      }
    }
  }

  foreach($return as &$term) {
    if (isset($parents_index[$term->tid]) && (is_null($max_depth) || $depth < $max_depth)) {
      $term->children = taxonomy_get_nested_tree($parents_index[$term->tid], $max_depth, $term->tid, $parents_index, $depth + 1);
    }
  }

  return $return;
}

function output_taxonomy_nested_tree($tree) {
    if (count($tree)) {
        foreach ($tree as $term) {

            $result_output ['id'] = $term->tid;
            $result_output ['name'] = $term->name;
            $result_output ['description'] = $term->description;

            $name = $term_name;
            if ($term->children) {
                // $output [] = output_taxonomy_nested_tree($term->children);
                $result_output ['child'] = output_taxonomy_nested_tree($term->children);
            }

            $output [] = $result_output;
        }
    }
    return $output;
}