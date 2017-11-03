<?php

/**
 * @file
 * Inc File for form in custom module.
 */

function _get_rendering_element() {
  $elements = [];
  
  $getForm = drupal_get_form('form_example_form');

  $elements['render_form'] = array(
    '#markup' => drupal_render($getForm),
  );
  return $elements;
}

function _get_tab_element() {}

function form_example_form($form, &$form_state, $node = NULL) {
  
  // if (empty($form_state['node']) && !empty($node)) {
  //   $form_state['node'] = $node;
  // }
  
  // if(empty($form_state['asset_cat_first_storage'])) {
  //     $tempIdentifer = uniqid();
  //     $form_state['asset_cat_first_storage'][]  = $tempIdentifer;
  //   }
   
  // if(empty($form_state['form_example_form_subject_value'])) {    
  //   $form_state['form_example_form_subject_value'] = array();   
  // }

  // if( (!empty($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'nod-list' ) || (!empty($form_state['node']))){    
  //   $triggeringElement = $form_state['triggering_element'];    

  //   if(!empty($form_state['node'])){
  //       $no_de = $form_state['node'];
  //       $edit_node = node_load($no_de->nid);
  //   }else{
  //     $edit_id = $triggeringElement['#value'];
  //     $edit_node = node_load($edit_id);
  //   }

  //   if(empty($form_state['node-state'])) {
  //     $form_state['node-state'] = '';
  //   }

  //   if((empty($form_state['node-state']) && $form_state['node-state'] != 'popup-modal' )){
  //     $form_state['asset_cat_first_storage'] = array();
  //     $form_state['form_example_form_subject_value'] = array();

  //     if(!empty( $edit_node->field_subject)){

  //       $subjects = $edit_node->field_subject;

  //       foreach($subjects['und'] as $subject => $val) { 
            
  //         $tempIdentifer = uniqid();  
           
  //         $form_state['asset_cat_first_storage'][] = $tempIdentifer;
        
  //         $form_state['form_example_form_subject_value'][$tempIdentifer] = $val['value']; 
           
  //       }
  //     }
  //   }

  // }
      
  $form['main-container'] = array(
    '#type' => 'container',
    '#attributes' => array('class'=>array('row')), 
    '#prefix' => '<div class="clearfix margin-bottom-10"><div id="project_test_data_container" class="project-detail project_test_data_container">', 
    '#suffix' => '</div></div>',
    '#process' => array('form_example_form_process'), 
  );

  $form['container-prefix'] = array(
    '#markup' => '<div id="main-container-multi-replace">',
  );

  $form['main-container-multi'] = array( 
    '#type' => 'container',
    '#attributes' => array('class'=>array('row')), 
    // '#prefix' => '<div class="clearfix margin-bottom-10"><div id="asset-container-section" class="project-detail project_test_data_container alert-container-section">', 
    '#prefix' => !empty($form_state['node']) ? '<div class="clearfix margin-bottom-10"><div id="asset-container-section-edit" class="project-detail project_test_data_container alert-container-section">' : '<div class="clearfix margin-bottom-10"><div id="asset-container-section" class="project-detail project_test_data_container alert-container-section">', 
    '#suffix' => '</div></div>',
    '#process' => array('form_example_multiple_subject_process'), 
  );

  $form['container-suffix'] = array(
    '#markup' => '</div>',
  );

  $form['edit-nodeId'] = array(
    '#type' => 'hidden', 
    '#attributes' => array('id' => 'edit-nodeId'),
  );

  $form['datatable_page_id']= array(
    '#type' => 'hidden',
    '#title_display' => 'invisible',
    '#attributes' => array('class'=> array('form-control datatable_page_id')),
  );

  $form['page_number_id']= array(
    '#type' => 'hidden',
    '#title_display' => 'invisible',
    '#attributes' => array('class'=> array('form-control page_number_id')),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Form Data'), 
    '#submit' => array('form_example_form_submit'),
    '#prefix' => '<div class="pull-left custom-test-program margin-bottom-10">',
    '#suffix' => '</div>', 
  );
  return $form;
}

function form_example_form_process($element, &$form_state) {


  $element['full-container']= array(
    '#type' => 'container',
    '#attributes' => array('class'=>array('col-md-12','col-sm-12','col-xs-12','project-details-left'),),
  );
  
  $query = db_select('node', 'n');
  $query->fields('n',array('nid','title'));
  $query->condition('n.type','product');
  $result = $query->execute();
  $subject_list = array('' => 'None');
    while($row = $result->fetchAssoc()) {
       $subject_list[ $row['nid'] ]= $row['title']; 
    }      

  if (!empty($form_state['node'])) {
    $node = $form_state['node'];
  }
  
  $element['full-container']['nod-list'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' =>$subject_list, 
    '#prefix' =>'<div style="display:none;">',
    '#suffix' =>'</div>',
    '#ajax' => array( 
      'callback' => 'form_example_edit_form_callback',  
      'wrapper' => 'project_test_data_container',  
    ),
    '#attributes' => array('class'=> array('edit-custom-node-select')),
    
  );
  
  // $element['full-container']['hidden_nid'] = array(
  //   '#type' => 'hidden', 
  //   '#value' => isset($_GET['n_id']) ? $_GET['n_id'] : '',
  // );

  // if(!empty(variable_get('form_id'))){
  //     $get_value = variable_get('form_id');
  // }else{
  //     variable_set('form_id', 1);
  //     $get_value = variable_get('form_id');
  // }

  // $get_id = sprintf('%04d' ,$get_value);
  // $uniq_var_id = 'STU' . $get_id . 'FORM';

  // $element['full-container']['frm_id'] = array(//you can find a list of available types in the form api
  //   '#type' => 'textfield', 
  //   '#title' => 'Form ID',
  //   '#size' => 25,
  //   '#default_value' => $uniq_var_id,
  //   '#maxlength' => 10,
  //   '#disabled' => TRUE, //make this field required 
  // );

  $element['full-container']['title'] = array(//you can find a list of available types in the form api
    '#type' => 'textfield', 
    '#title' => 'Name / Title',
    '#size' => 10,
    '#attributes' => array('class' => array('titleclass')),
    '#default_value' => isset($node->title) ? $node->title : '',
    '#maxlength' => 10,
    '#required' => TRUE, //make this field required 
  );

  $element['full-container']['copy'] = array(  
    '#type' =>'checkbox', 
    '#title' => t('Send me a copy. (Testing Of checkbox)'),
    // '#default_value' => isset($node->field_boolean_on_off_check_box_['und']['0']['value']) ? $node->field_boolean_on_off_check_box_['und']['0']['value'] : 0,
  );

  $element['full-container']['high_school']['tests_taken'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array(t('SAT'), t('ACT'))),
    '#title' => t('What standardized tests did you take? 
                  (Testing Of Multi CheckBoxes)'),
  );

  
  $currentDate = array('year' => 2017, 'month' => 2, 'day' => 15);

  $element['full-container']['date_field'] = array(
      '#type' => 'date',
      '#title' => 'Choose Date (Testing of Date)',
  );

  $element['full-container']['vertical_tabs_container'] = array(

    '#type' => 'vertical_tabs', 
    '#title' => t('Verical Tab Example (Testing Of Verical Tabs)'), 
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );


  $element['full-container']['contact'] = array(

    '#type' => 'fieldset', 
    '#title' => t('Contact settings (Testing Of FieldSet)'), 
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'vertical_tabs_container',
    '#description' => 'Contact setting description'.'<h1> hello </h1>'.
      '<ul>'
      // .' &lt;'.'li' .' &gt;'. ' style = "color: red;">Hello</li>
      .' &lt;'.'li'.' style = "color: red;"'.'&gt;'.'Hello'.' &lt;'.'li' .'&gt;'.'
      <li>Hello</li>
      <li>Hello</li>
      <li>Hello</li>
      </ul>',
  );

  $element['full-container']['contact']['fieldset_text'] = array(

    '#type' => 'textfield', 
    '#title' => t('Field Set Text Settings (Testing Of FieldSet Text)'),
    '#description' => 'FieldSet Text',
  );

  // $image_options = array();
  // foreach ($images as $img) {
  //   $image_options[$img['fid']] = array(
  //     'image_name' => $img['image_name'],
  //   );
  // }

  // $element['full-container']['image_table'] = array(
  //   '#type' => 'tableselect',
  //   '#header' => $image_header,
  //   '#options' => $image_options,
  //   '#empty' => t('No users found'),
  // );  

  $edit_id = '';

  if(( !empty($form_state['triggering_element']['#name']) && $form_state['triggering_element']['#name'] == 'nod-list' ) || (!empty($form_state['node'])) ) {    

    if(!empty($form_state['node'])){
      $no_de = $form_state['node'];
      $edit_id = $no_de->nid;

    }elseif (($form_state['triggering_element']['#name'] == 'nod-list')){
      
      $triggeringElement = $form_state['triggering_element'];    
      
      if(isset($triggeringElement['#value']))
        $edit_id = $triggeringElement['#value'];

    }else{
      $edit_id = NULL;      
    }
  }

  if(!empty($node->nid)){
    $edit_id = $node->nid;
  }

  $element['full-container']['get_image_table'] = array(
    '#markup' => _get_image_table_data($edit_id),
    // '#prefix' => !empty($form_state['node']) ? '<div class="image_main_container_edit"> <div class="image-listing-edit" id="form-example-image-listing-edit">' : '<div class="image_main_container"> <div class="image-listing" id="form-example-image-listing">',
    '#prefix' => '<div class="image_main_container"> <div class="image-listing" id="form-example-image-listing">',
    '#suffix' => '</div></div>'
  );

  $common_arr = array('year' => 2017, 'month' => 2, 'day' => 15);
  $common_arr2 = 'hello';
  $element['full-container']['testfield'] = array(
    '#type' => 'textfield', //you can find a list of available types in the form api
    '#title' => 'Test Field (with field prefix)',
    '#size' => 10,
    '#maxlength' => 10,
    // '#required' => TRUE, //make this field required 
    // '#field_prefix' => url(NULL, NULL, NULL, TRUE) . (variable_get('clean_url', 0) ? '' : '?q=')
    '#field_prefix' => url(NULL, $common_arr, NULL, TRUE) . (variable_get('clean_url', 0) ? '' : '?q=')
  );
  return $element;
}

function dropdown_page(){
  echo "string";
  exit;
}

// function form_example_multiple_subject_process($element,&$form_state) {
function form_example_multiple_subject_process($element, &$form_state) {

  $storage = $form_state['asset_cat_first_storage'];

  $element['#storage'] = $storage;

  $element['fieldset_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Your Subject'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE, 
  );
  
  foreach ($storage as $key => $uniqId) {    
    $element['fieldset_container']['test_name'][$uniqId] = array(
      '#type' => 'textfield',
      '#attributes'=> array('class'=> array('form-control','test_name','bg-yellow')),
      '#parents' => array('postedData',$uniqId,'test_name'),
      '#prefix' => '<div class="pull-left">',
      '#suffix' => '</div>',
    );
    
    $element['fieldset_container']['remove'][$uniqId] = array(
      '#type' => 'submit',
      '#value' => 'Remove',
      '#name' => $uniqId,
      '#attributes'=> array('style'=> array('font-family:FontAwesome;'),'class'=>array('alert-container-section', 'workflow-remove-submit')),
      '#parents' => array('postedData', $uniqId, 'remove'),      
      '#submit'=> array('remove_subject_asset_category_handler'),
      '#ajax' => array(
        'callback' => 'asset_category_row_callback_for_remove',
        'wrapper' => !empty($form_state['node']) ? 'asset-container-section-edit' : 'asset-container-section',
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array('type'=> 'throbber', 'message'=> NULL),
      ),
      '#limit_validation_errors' => array(),
    );
    
    if(isset($form_state['form_example_form_subject_value'][$uniqId])) {
      $stdefault = $form_state['form_example_form_subject_value'][$uniqId];
      $element['fieldset_container']['test_name'][$uniqId]['#default_value'] = $stdefault;
    }
    //dpm($element['fieldset_container']);
    // drupal_set_message(print_r($form_state['form_example_form_subject_value'],1));
  }

  $element['workflow_add_new_row'] = array(
    '#type' => 'submit',
    '#value' => 'Add New Subject',
    '#name' => 'workflow_add_new_row',
    '#attributes'=> array('class'=>array('routing-workflow-action','assets-workflow-action-add-new-assets'),'title'=>'Add New Detail'),
    '#submit'=> array('custom_multiple_subject_add_submit_callback'),
    '#ajax' => array(
      'callback' => 'custom_multiple_subject_add_ajax_callback',
      'wrapper' => !empty($form_state['node']) ? 'asset-container-section-edit' : 'asset-container-section',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type'=> 'throbber', 'message'=> NULL),
    ),
    '#prefix' => '<div class="table-footer-custom clearfix margin-bottom-10">',
    '#suffix' => '</div>',
    '#limit_validation_errors' => array(),
  );

  return $element;
}

function form_example_other_field_process($form, &$form_state) {

  $form_state['dropdown_element']['#name'] = 'new-text-field';
  $form_state['rebuild'] = TRUE;
}

function form_example_other_field_process_callback($form, &$form_state) {
  $form['other_field'] = array(
    '#type' => 'textfield',
    '#attributes'=> array('class'=> array('form-control','other_field','bg-yellow')),
    // '#parents' => array('postedData',$uniqId,'other_field'),
    '#prefix' => '<div class="pull-left">',
    '#suffix' => '</div>',
  );
  // $form_state['rebuild'] = TRUE;
  return $form;
}

function custom_multiple_subject_add_submit_callback($form, &$form_state) {
  $triggerElement = $form_state['triggering_element'];
  $tempIdentifer = uniqid();
  $form_state['asset_cat_first_storage'][]  = $tempIdentifer;    

  if(!empty($form_state['node']))
    $form_state['node-state'] = 'popup-modal'; 

  $form_state['rebuild'] = TRUE;
}

function custom_multiple_subject_add_ajax_callback(&$form, &$form_state) {  
  return $form['main-container-multi'];
}

function remove_subject_asset_category_handler(&$form,&$form_state) {
  $triggering_element = $form_state['triggering_element'];
  $identifier = $triggering_element['#parents'][1];
  if(isset($form_state['asset_cat_first_storage'])){
    foreach ($form_state['asset_cat_first_storage'] as $key => $value) {
      if($form_state['asset_cat_first_storage'][$key] == $identifier){
        unset($form_state['asset_cat_first_storage'][$key]);
        unset($form_state['form_example_form_subject_value'][$identifier]);
      } 
    }             
  }   
  $form_state['rebuild'] = TRUE;
}

function asset_category_row_callback_for_remove(&$form, &$form_state) {
  return $form['main-container-multi'];
}

function form_example_form_validate($form, &$form_state) {
  
  $num_files = count($_FILES['files']['name']);
  for ($i = 0; $i < $num_files; $i++) {
    $file = file_save_upload($i, array(
      'file_validate_is_image' => array(),
      'file_validate_extensions' => array('png gif jpg jpeg'),
    ));

    if ($file) {
      if ($file = file_move($file, 'public://image_example_images/')) {
        $form_state['values']['file'][$i] = $file;
      } else {
        form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
      }
    } else {
      form_set_error('file', t('No file was uploaded.'));
    }
  }
}

//stdClass Object Converting to arrray;
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

function form_example_form_submit($form, &$form_state) {

  if(!empty($form_state['node'])){
    $nod = $form_state['node'];
    $node_id = $nod->nid; 
  }else{
    $node_id = $form_state['values']['edit-nodeId'];
  }

  global $user;

  if(!empty($node_id)){

    $node = [];
    $node = node_load($node_id);
    // add node properties
    $node->title = $form_state['values']['title'];
    $node->body['und']['0'] = $form_state['values']['description'];

    $node->field_boolean_on_off_check_box_['und']['0']['value'] = $form_state['values']['copy'];
    $node->field_list_integer_['und']['0']['value'] = $form_state['values']['active2'];

    // $image_file_count = count($form_state['values']['file']);

    // for($i=0; $i< $image_file_count; $i++){
    //   $newNode->field_image_testing['und'][] = (array)$form_state['values']['file'][$i];     
    // }

    $subjects = $form_state['values']['postedData'];

    $count = count($node->field_subject['und']);

    $i = 0;
    foreach( $subjects as $subject => $val ) {

      if($i < $count)
        $node->field_subject['und'][$i]['value'] = $val['test_name'];
      else   
        $node->field_subject['und'][]['value'] = $val['test_name'];   
        
      $i++;
    }
    
    $year = $form_state['values']['date_field']['year'] ;
    $month = $form_state['values']['date_field']['month'] ;
    $day = $form_state['values']['date_field']['day'] ;
    $currentDate = $day."-".$month."-".$year ;
    $node->field_date_iso_format_testing['und']['0']['value'] = $currentDate ;

    $image_file_count = count($form_state['values']['file']);

    for($i=0; $i< $image_file_count; $i++){
      $node->field_image_testing['und'][] = (array)$form_state['values']['file'][$i];     
    }

    // if(!empty(variable_get('form_id'))){
    //   $get_value = variable_get('form_id');
    //   $get_value++;
    //   variable_set('form_id', $get_value);
    // }else{
    //   variable_set('form_id', 1);
    // }

    node_save($node);
    drupal_set_message('My Edited Node No :'.$node->nid);


    $form['#attached']['js'] = array(
       drupal_get_path('module', 'form_example') . '/js/form_alter.js',
    );
    
    $tempIdentifer = uniqid();
    $form_state['input'] = array();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    $form_state['rebuild'] = TRUE;

    if((isset($form_state['ajax']))?(($form_state['ajax']==1)?true:false):false) {
        $form_state['ajax_commands'][] = ctools_modal_command_dismiss();

        if(!empty($form_state['values']['datatable_page_id'])){

          $pageNo = $form_state['values']['datatable_page_id'];

          if($pageNo == 0 || $pageNo == 1) {
            $from = 0;
            $to = 10;
          }
          else {
            $from = ($pageNo - 1) * 10;
            $to = $pageNo * 10;
          }
          
          $form_state['ajax_commands'][] = ajax_command_html('#form-example-data-listing', _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$from, $to));
        }

        // $form_state['ajax_commands'][] = ajax_command_html('#form-example-data-listing', _get_form_posted_data());
    }

  }else{

    // add node properties
    $newNode = (object) NULL;
    $newNode->type = 'newsletter';
    $newNode->title = $form_state['values']['title'];
    $newNode->body['und']['0'] = $form_state['values']['description'];
    
    $newNode->field_boolean_on_off_check_box_['und']['0']['value'] = $form_state['values']['copy'];
    
    $newNode->field_list_integer_['und']['0']['value'] = $form_state['values']['active2'];
    
    $image_file_count = count($form_state['values']['file']);

    for($i=0; $i< $image_file_count; $i++){
      $newNode->field_image_testing['und'][] = (array)$form_state['values']['file'][$i];     
    }

    $subjects = $form_state['values']['postedData'];

    foreach( $subjects as $subject => $val ) {
      $newNode->field_subject['und'][]['value'] = $val['test_name'];   
    }  

    $year = $form_state['values']['date_field']['year'] ;
    $month = $form_state['values']['date_field']['month'] ;
    $day = $form_state['values']['date_field']['day'] ;
    $currentDate = $day."-".$month."-".$year ;
    $newNode->field_date_iso_format_testing['und']['0']['value'] = $currentDate ;
    
    $newNode->uid = $user->uid;
    $newNode->created = strtotime("now");
    $newNode->status = 1;
    $newNode->language = LANGUAGE_NONE;
    // $newNode->changed = strtotime("now");
    // $newNode->comment = 0;
    // $newNode->promote = 0;
    // $newNode->moderate = 0;
    // $newNode->sticky = 0;
    
    // exit;
    if(!empty(variable_get('form_id'))){
      $get_value = variable_get('form_id');
      $get_value++;
      variable_set('form_id', $get_value);
    }else{
      variable_set('form_id', 1);
    }
    
    node_save($newNode);
    // drupal_set_message('My Node No :'.$newNode->nid);

    $tempIdentifer = uniqid();
    $form_state['input'] = array();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][] = $tempIdentifer;
    $form_state['rebuild'] = TRUE;

    if((isset($form_state['ajax']))?(($form_state['ajax']==1)?true:false):false) {
        $form_state['ajax_commands'][] = ctools_modal_command_dismiss();
        
        if(!empty($form_state['values']['datatable_page_id'])){
          
          $pageNo = $form_state['values']['datatable_page_id'];

          if($pageNo == 0 || $pageNo == 1) {
            $from = 0;
            $to = 10;
          }
          else {
            $from = ($pageNo - 1) * 10;
            $to = $pageNo * 10;
          }
          
          $form_state['ajax_commands'][] = ajax_command_html('#form-example-data-listing', _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$from, $to));
        }

        // $form_state['ajax_commands'][] = ajax_command_html('#form-example-data-listing', _get_form_posted_data());
    }
  }
}

function form_example_save_newsletter_data($form, &$form_state) {
    $tempIdentifer = uniqid();
    $form_state['asset_cat_first_storage'] = array();
    $form_state['asset_cat_first_storage'][]  = $tempIdentifer;
    $commands = array();    
    // $commands[] = ajax_command_invoke('#edit-frm-id', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-title', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-price', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-description-value', 'val', array(''));

    $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data());
    $commands[] = ajax_command_html('#main-container-multi-replace', render($form['main-container-multi']));

    return array('#type' => 'ajax', '#commands' => $commands); 
}

function form_example_edit_form_callback($form, $form_state) {
  
  $triggeringElement = $form_state['triggering_element'];

  $form_state['form_example_subject_storage'] = array();
  
  $form_state['form_example_form_subject_value'] = array();
  
  $commands = array();
  
  if($form_state['triggering_element']['#type']=='select' && $form_state['triggering_element']['#name']=='nod-list' ) {

    $edit_id = $triggeringElement['#value'];  
    $edit_node = node_load($edit_id);   
    $commands[] = ajax_command_invoke('#edit-nodeId', 'val', array($edit_id));
    $commands[] = ajax_command_invoke('#edit-title', 'val', array($edit_node->title));
    $commands[] = ajax_command_invoke('#edit-price', 'val', array(''));
    $commands[] = ajax_command_invoke('#edit-description-value', 'val', array($edit_node->body['und'][0]['value']));

    $radio = $edit_node->field_list_integer_;
    $val = $radio['und'][0]['value'];
    $commands[] = ajax_command_invoke('.form-custom-radio input[value="'.$val.'"]', 'attr',array('checked', 'true'));
    
    $commands[] = ajax_command_replace('#asset-container-section', render($form['main-container-multi']));

    // $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data());
    // print "<pre>";
    // print_r($form_state['values']['page_number_id']);
    // exit;
    if(!empty($form_state['values']['datatable_page_id'])){

      $pageNo = $form_state['values']['datatable_page_id'];

      if($pageNo == 0 || $pageNo == 1) {
        $from = 0;
        $to = 10;
      }
      else {
        $from = ($pageNo - 1) * 10;
        $to = $pageNo * 10;
      }

      if(!empty($form_state['values']['page_number_id'])){
        $from = 'select';
        $to = $form_state['values']['datatable_page_id'];
      }

      $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$from, $to));
    }

    $commands[] = ajax_command_html('#form-example-image-listing', _get_image_table_data($edit_id));
  }  

  return array('#type' => 'ajax', '#commands' => $commands);

}

function form_example_delete_callback($node_id, $no_js = NULL) {
  ctools_include('ajax');
  $commands = array();
  node_delete($node_id);  

  $from = 0;
  $to = 10;
  // if(!empty($form_state['values']['datatable_page_id'])){
  if(!empty(variable_get('page_no'))){

    $pageNo = variable_get('page_no');

    if($pageNo == 0 || $pageNo == 1) {
      $from = 0;
      $to = 10;
    }
    else {
      $from = ($pageNo - 1) * 10;
      $to = $pageNo * 10;
    }
    
    // $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$from, $to));
  }

  $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$from, $to));

  print ajax_render($commands); // this function exits.
  exit;
  /*We can use both exit or return bcoz after rendring of html and echo of it, function should to be halt and return some value*/
  // return;
}

function form_example_update_callback($node_id, $val ,$no_js = NULL) {
  ctools_include('ajax');
  $commands = array();

  if(!empty($node_id)){
    $node = [];
    $node = node_load($node_id);
    
    switch ($val) {
      case 1:
        $status_name = 'Approved';
        break;
      case 2:
        $status_name = 'Canceled';
        break;
      case 3:
        $status_name = 'Rejected';
        break;
      default:
        $status_name = 'Draft';
        break;
    }
    
    // add node properties
    $node->field_flag_value['und']['0']['value'] = $val;
    $node->field_status['und']['0']['value'] = $status_name;
    node_save($node);
  }

  $from = 0;
  $to = 10;
  // if(!empty($form_state['values']['datatable_page_id'])){
  if(!empty(variable_get('page_no'))){

    $pageNo = variable_get('page_no');

    if($pageNo == 0 || $pageNo == 1) {
      $from = 0;
      $to = 10;
    }
    else {
      $from = ($pageNo - 1) * 10;
      $to = $pageNo * 10;
    }
    
    // $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$from, $to));
  }

  $commands[] = ajax_command_html('#form-example-data-listing', _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$from, $to));

  print ajax_render($commands); // this function exits.
  exit;
  /*We can use both exit or return bcoz after rendring of html and echo of it, function should to be halt and return some value*/
  // return;
}

/**
 * Ajax menu callback.
 */
function form_example_callback($node, $ajax = NULL) {
  if ($ajax) {
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'title' => t('Form Example Modal Form'),
      'node' => $node,
    );

    // Use ctools to generate ajax instructions for the browser to create
    // a form in a modal popup.
    // $output = ctools_modal_form_wrapper('mymodule_form', $form_state);
    $output = ctools_modal_form_wrapper('form_example_form', $form_state);
    // If the form has been submitted, there may be additional instructions
    // such as dismissing the modal popup.
    if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }

    // Return the ajax instructions to the browser via ajax_render().
    print ajax_render($output);
    drupal_exit();
  }
  else {
    // return drupal_get_form('mymodule_form');
    return drupal_get_form('form_example_form', $node);
  }
}

/**
 * Helper function to make a link.
 */
function _form_example_make_link($link_text = '', $nids) {
  // Set a default value if no text in supplied.
  if (empty($link_text)) {
    $link_text = 'Edit Using Pop Up';
  }

  return '<div id="magical-modal-link">' . l($link_text, 
                                    'formmodule/'.$nids.'/nojs', array(
                                      'attributes' => array(
                                        'class' => array(
                                            'ctools-use-modal'
                                            // 'edit-form-detail'
                                          ),
                                        'data-value' => $nids
                                      )
                                    )
                                  ) . 
          '</div>';
}

/**
 * Getting Image Tabel To show at edit time.
 */
function _get_image_table_data($edit_id){
  if(!($edit_id == NULL) && (!empty($edit_id))){
    drupal_add_js( drupal_get_path('module', 'form_example') . '/js/form_alter.js');

    $rows = array();
    $header = array(
      array('data'=>'Sr. No'),
      array('data'=>'File Detail'),
      array('data'=>'Remove Link'),
    );  

    $nodeData = node_load($edit_id);
    $nid = $nodeData->nid;

    if(!empty($nodeData->field_image_testing['und'])){
      $image_arr = $nodeData->field_image_testing['und'];
      
      $i = 1;
      foreach ($image_arr as $key => $imag_uri_value) {

        
        $img_url = file_create_url($imag_uri_value['uri']);
        $img_name = $imag_uri_value['filename'];
        $img_id = $imag_uri_value['fid'];
        
        $remove_link = l('Remove', 
                        url('image-delete/' . $img_id.'/'. $nid .'/nojs'), 
                        array( 
                          'external'=>TRUE, 
                          'attributes'=> array(
                            'class'=>array('image-detail-delete','use-ajax')
                            )
                          )
                        );

        $rows[] = array(
          'data' => array(  
            array('data'=>$i),    
            array('data'=>'<div> <img src="'.$img_url.'" alt="photo" style="width:65px;height:65px"> </div> 
                          <div>'. $img_name .'</div>','class'=> array('text-left')),      
            array('data'=>$remove_link),
          ),  
        );
        
        $i++;

      }
    }

    return theme('table', array('header'=>$header,'rows'=>$rows,'empty'=>'No records yet'));
  }else{
    return ;
  }

}

function form_example_image_delete_callback($file_id, $node_id, $no_js = NULL) {

  ctools_include('ajax');
  $commands = array();

  $node = node_load($node_id);
  $f_id = $file_id;

  $img_arr = $node->field_image_testing;
  $und_inc = 0;
  // Extracting Position Of Image.
  foreach ($img_arr['und'] as $key => $value) {

    // Checking Position
    if($value['fid'] == $f_id){
      $pos_count = $und_inc;

      // Deleting entry from table
      // $file_delete_query = db_delete('field_data_field_image_testing')
      //       ->condition('entity_id', $node_id)
      //       ->condition('field_image_testing_fid', $value['fid']);

      // $file_delete_query->execute();

      // if($und_inc > 0)
      //   $und_inc--;
    
    }   

    // Updating table for delta position
    // $update_query = db_update('field_data_field_image_testing')
    //       ->fields(array('delta' => $und_inc))
    //       ->condition('entity_id', $node_id)
    //       ->condition('field_image_testing_fid', $value['fid'])
    //       ->condition('field_image_testing_fid', $f_id,'!=');
        
    // $update_query->execute();
    

    $und_inc++;
  }

  // $count = 1;

  // // Delete rows that have a exact or less value to prevent empty rows.
  // $file_del_query = db_delete('file_usage')
  //   ->condition('fid', $f_id)
  //   ->condition('id', $node_id);
  
  // if ($count) {
  //   $file_del_query->condition('count', $count, '<=');
  // }

  // $file_del_query->execute();


  // Flle loding and deleting with node updaing file
  $file = file_load($node->field_image_testing['und'][$pos_count]['fid']);
  unset($node->field_image_testing['und'][$pos_count]);
  node_save($node);

  file_delete($file);

  // If the row has more than the specified count decrement it by that number.
  // if (!$result && $count > 0) {
  //   $query = db_update('file_usage')
  //     ->condition('fid', $file->fid)
  //     ->condition('id', $node_id);
    
  //   $query->expression('count', 'count - :count', array(':count' => $count));
  //   $query->execute();
  // }
  
  $commands[] = ajax_command_replace('#form-example-image-listing', _get_image_table_data($node_id));

  print ajax_render($commands); // this function exits.
  exit;

  /*We can use both exit or return bcoz after rendring of html and echo of it, function should to be halt and return some value*/
  // return;
}

// function _get_form_posted_data() {
function _get_form_posted_data($projectId = 1,$filterFor = '', $filterVal='',$ajax=FALSE,$startRange=0, $endRange=10) {
  drupal_add_js( drupal_get_path('module', 'form_example') . '/js/form_alter.js');

  $rows = array();
  $header = array(
    array('data'=>'Sr. No'),
    array('data'=>'Title'),
    array('data'=>'Photo'),
    array('data'=>'Edit Link'),
    array('data'=>'Delete Link'),
    array('data'=>'Edit Link Using Pop Up Modal'),
    array('data'=>'Actions'),
  );  
  $i = 1;

  $query = db_select('node','n');
  $query->fields('n');
  $query->condition('n.type','newsletter');

  // Initialize the pager in query
  // $query = $query->extend('TableSort')->extend('PagerDefault')->limit(25);
  
  // Initialize the pager by second method

  $query_data = clone $query;
  $execute_data = $query_data->execute();

  $row_count = $execute_data->rowCount();
  
  if(($startRange == 0) || ($startRange == 'select')){

    switch($startRange){
      case 'select':
        $getEtcNodes = $query->range(0,$endRange)->execute()->fetchAll();
        break;
      default:
        $getEtcNodes = $query->range($startRange,10)->execute()->fetchAll();
    }
  }else{
    // $getEtcNodes = $query->range($endRange,10)->execute()->fetchAll();
    if(!empty(variable_get('to'))){
      $to = variable_get('to');
      $getEtcNodes = $query->range($startRange,$to)->execute()->fetchAll();
    }
    else{
      $getEtcNodes = $query->range($startRange,10)->execute()->fetchAll();
    }
  }

  $getNodeIds = array();
  foreach ($getEtcNodes as $res_arr) {
      $getNodeIds[] = $res_arr->nid;
  }

  foreach($getNodeIds as $delta => $nids) {
    $loadNodeData = node_load($nids);
    
    $delete_link = l('Delete', 
                    url('node-delete/' . $nids.'/nojs'), 
                    array( 
                      'external'=>TRUE, 
                      'attributes'=> array(
                        'class'=>array('node-detail-delete','use-ajax')
                        )
                      )
                    );
    
    $edit_link = l('Edit', 
                  'javascript:void(0)', 
                  array(
                    'attributes' => array(
                      'class' => array(
                        'edit-form-detail'
                        ),
                      'data-value' => $nids 
                      )
                    )
                  );

    $flag_val = intval($loadNodeData->field_flag_value);
    $status = ['Draft','Approve','Canceled','Reject'];

    if(isset($loadNodeData->field_status['und']['0']['value']))
      $status_name = $loadNodeData->field_status['und']['0']['value'];
    else
      $status_name = "Draft";

    switch ($flag_val) {
      case 1:
        $status = array_diff($status, array('Approve'));
        // $status_name = 'Approved';
        break;
      case 2:
        $status = array_diff($status, array('Canceled'));
        // $status_name = 'Canceled';
        break;
      case 3:
        $status = array_diff($status, array('Reject'));
        // $status_name = 'Rejected';
        break;
      default:
        $status = array_diff($status, array('Draft'));
        // $status_name = 'Draft';
        break;
    }

    $all_link = '';
    foreach ($status as $key => $value) {

      switch ($value) {
        case 'Approve':
          $flag_pass_val = 1;
          break;
        case 'Canceled':
          $flag_pass_val = 2;
          break;
        case 'Reject':
          $flag_pass_val = 3;
          break;
        default:
          $flag_pass_val = 0;
          break;
      }

      $all_link .= l($value,
                      url('node-update/' . $nids.'/'.$flag_pass_val.'/nojs'), 
                      array( 
                        'external'=>TRUE, 
                        'attributes'=> array(
                          'class'=>array('node-detail-update','dropdown-item','use-ajax')
                          )
                        )
                      );
    }

    $actions = '<div class="dropdown show">'.
                  l($status_name,
                    url('https://example.com'), 
                    array(
                      'attributes' => array(
                        'class' => array(
                          'btn btn-secondary dropdown-toggle'
                          ),
                        'id' => array(
                          'dropdownMenuLink'
                          ),
                        'data-toggle  ' => array(
                          'dropdown'
                          ),
                        'aria-haspopup' => array(
                          'true'
                          ),
                        'aria-expanded' => array(
                          'false'
                          ),
                        )
                      )
                    )
                  .'
                  <div class="dropdown-menu" aria-labelledby="dropdownMenuLink">
                  '.$all_link.'
                  </div>
                </div>';

    ctools_include('modal');
    ctools_modal_add_js();

    $edit_link_for_popup = _form_example_make_link('Edit Using Pop Up', $nids);
    
    if(isset($loadNodeData->field_image_testing['und']['0']['uri']))
      $img_url = file_create_url($loadNodeData->field_image_testing['und']['0']['uri']);
    else
      $img_url = "http://no_image";

    $rows[] = array(
      'data' => array(  
        array('data'=>$i),    
        array('data'=>$loadNodeData->title,'class'=> array('text-left')),        
        array('data'=>'<img src="'.$img_url.'" alt="photo" style="width:100px;height:125px">','class'=> array('text-left')),      
        array('data'=>$edit_link),        
              // 'target' => '_blank'
              // 'onclick' => 'if(!confirm("Really Edit?")){
              //   return false;
              // }'
            // 'ajax' => array(
              // 'callback' => '_print_hello ',
              // 'wrapper' => 'channel_container_' . $channel->nid
            // )
        array('data'=>$delete_link),
          // array('attributes' => array(
          //     'onclick' => 'if(!confirm("Really Delete?")){
          //       return false;
          //     }'
          //     ),
          //     'ajax' => array(
          //         'callback' => 'form_example_delete_node',
          //         'arguments' => $nids
          //       )
          //   )
          // )
        array('data'=>$edit_link_for_popup),
        array('data'=>$actions),
                  
      ),  
    );
    
    $i++;
  }
  /*
  By default table with pager
  **/
  // $per_page = 10;
  // // Initialize the pager
  // $current_page = pager_default_initialize(count($rows), $per_page);
  // // Split your list into page sized chunks
  // $chunks = array_chunk($rows, $per_page, TRUE);
  // // Show the appropriate items from the list
  // $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
  // // Show the pager
  // $output .= theme('pager', array('quantity',count($rows)));

  // return $output;

  /*
  By default table
  **/
  // return theme('table', array('header'=>$header,'rows'=>$rows,'empty'=>'No records yet'));

  /*
  Using pager with query
  **/
  // return theme_table(
  //   array(
  //     "header" => $header,
  //     "rows" => $rows,
  //     "attributes" => array(),
  //     "sticky" => true, // Table header will be sticky
  //     "caption" => "",
  //     "colgroups" => array(),
  //     "empty" => t("Table has no row!") // The message to be displayed if table is empty
  //   )
  // ).theme("pager");

  /*
  Using ajax pager
  **/
  $outputBefore = _get_pager_option_data_list($projectId,'asset');
  if(!$ajax){
    $output = theme('table', array('header'=>$header,'rows'=>$rows,'empty'=>'No records yet','attributes'=>array('class'=>array('table','custom-table','m6connect-custom-program-table','table-hover','table-bordered','text-center'))));       
  }else{
    $output = theme('feedback_table_rows', array('header' => array(), 'rows' => $rows));
  }

  $pagerGene = '';
  if($row_count > 10) {  
    $pagerGene .= '<div class="item-list"><ul class="pager">';
    $pagerGene .= '<li class="pager-current">'.l('<< First', 'get-next-page-data-example/nextpage/0/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-0')))).'</li>';
    if($row_count > 10) {
      $getpageLimit = $row_count/10;
      
      if($startRange == 'select'){
        $getpageLimit = $row_count/$endRange;
        $getpageLimit = ceil($getpageLimit);
        variable_set('getpageLimit', $getpageLimit);
      }
      
      if(!empty(variable_get('to'))){
        $getpageLimit = $row_count/$endRange;
      }

      $getpageLimit = ceil($getpageLimit);
      
      if(!empty(variable_get('getpageLimit')))
        $getpageLimit = variable_get('getpageLimit');

      variable_del('getpageLimit');
    }
    for($i = 1; $i<=$getpageLimit; $i++) {
      $pagerGene .= '<li class="pager-current">'.l($i, 'get-next-page-data-example/nextpage/'.$i.'/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-'.$i)))).'</li>';
    }
    $pagerGene .= '<li class="pager-current">'.l('Last >>', 'get-next-page-data-example/nextpage/'.$getpageLimit.'/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-0')))).'</li>';
    $pagerGene .= '</ul></div>';
  }
  variable_del('to');
  // return $output.$pagerGene;
  return $outputBefore.$output.$pagerGene;
}

function _get_pager_option_data_list($projectId,$type) {
  $output = '<div class="item-data-list" style="display:none;"><ul class="pager-list">';
  $output .= '<li class="pager-no-10">'.l(10, 'get-total-number-of-data/0/10/'.$type.'/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-10')))).'</li><li class="pager-no-20">'.l(20, 'get-total-number-of-data/0/20/'.$type.'/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-20')))).'</li><li class="pager-no-30">'.l(30, 'get-total-number-of-data/0/30/'.$type.'/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-30')))).'</li><li class="pager-no-50">'.l(50, 'get-total-number-of-data/0/50/'.$type.'/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-50')))).'</li><li class="pager-no-100">'.l(100, 'get-total-number-of-data/0/100/'.$type.'/'.$projectId.'/nojs', array('attributes' => array('class' => array('use-ajax','page-range-100')))).'</li></ul></div>';
  $output .= '<div class="margin-bottom-15"><div class="item-list-select input-group"><span class="input-group-addon">Show Entries: </span>
          <select class="pager-load-more-to" name="pager-navigation-control" for="'.$projectId.'">
              <option value="10">10</option>
                <option value="20">20</option>
              <option value="30">30</option>
              <option value="50" selected="selected">50</option>
              <option value="100">100</option>
              </select>';
  $output .= '</div></div>';
  return $output;
}

function get_next_page_data_records_example($type,$pageNo,$proId, $ajax = NULL) {   
  ctools_include('ajax');    
  if($pageNo == 0 || $pageNo == 1) {
    $from = 0;
    $to = 10;
  }
  else {
    $from = ($pageNo - 1) * 10;
    $to = $pageNo * 10;
  }
  $commands = array();
  
  if($type == 'nextpage') {
    // $output = get_commitment_listing_table_data($proId,'','', $ajax=FALSE, $from, $to);
    $output = _get_form_posted_data($proId,'','', $ajax=FALSE, $from, $to);
    $commands[] = ajax_command_html('#form-example-data-listing', $output);
  }
  
  $commands[] = ajax_command_invoke("input.datatable_page_id", 'val',array($pageNo));  
  variable_set('page_no', $pageNo);
  print ajax_render($commands);
  exit;   
}

function get_total_number_data_records_example($start,$end,$type,$proId,$ajax = NULL) {
  ctools_include('ajax');
  $commands = array();

  if($type == 'nextpage') {
    // $output = get_commitment_listing_table_data($proId,'','', $ajax=FALSE, 'select', $end);
    $output = _get_form_posted_data($proId,'','', $ajax=FALSE, $start, $end);
    $commands[] = ajax_command_html('#form-example-data-listing', $output);
  }

  if($type == 'asset') {

    $from = $start;
    $to = $end;
    // if(!empty($form_state['values']['datatable_page_id'])){
    if(!empty(variable_get('page_no'))){

      $pageNo = variable_get('page_no');

      if($pageNo == 0 || $pageNo == 1) {
        $from = 0;
        $to = $end;
      }
      else {
        $from = ($pageNo - 1) * 10;
        // $to = $pageNo * 10;
        $to = $from + $end;
      }
    }

    variable_set('to', $to);

    if($from == 0)
      $output = _get_form_posted_data($proId,'','', $ajax=FALSE, 'select', $to);
    else
      $output = _get_form_posted_data($proId,'','', $ajax=FALSE, $from, $to);
      
    $commands[] = ajax_command_html('#form-example-data-listing', $output);
  }

    
  // $commands[] = ajax_command_invoke("select.pager-load-more-to", 'val',array($end));
  $commands[] = ajax_command_invoke("select.pager-load-more-to", 'val',array($to));
  // variable_set('page_no', $end);
  print ajax_render($commands);
  exit;   
}