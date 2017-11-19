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

      // Mail
      $to = "anuj957139@gmail.com";
      $subject = "Enquiry successfully received";

      $message = '
      <html>
          <head></head>
          <body>
              <style type="text/css"> * {
                  font-size: 14px;
                  font-family: sans-serif;
                  line-height: 20px;
              }
              </style>>
              <table border="0" cellpadding="0" cellspacing="0" width="100%">
                  <tbody>
                      <tr>
                          <td width="100%" align="center" valign="top" bgcolor="#eeeeee" height="20"></td>
                      </tr>
                      <tr>
                          <td bgcolor="#eeeeee" align="center" style="padding:0px 15px 0px 15px;" class="yiv3733200406section-padding">
                              <table bgcolor="#ffffff" border="0" cellpadding="0" cellspacing="0" width="100%" style="max-width:600px;" class="yiv3733200406responsive-table">
                                  <tbody>
                                      <tr>
                                          <td>
                                              <table width="100%" border="0" cellspacing="0" cellpadding="0">
                                                  <tbody>
                                                      <tr>
                                                          <td align="center" style="padding:40px 40px 0px 40px;">
                                                              <a rel="nofollow" target="_blank" href="http://www.digitaserve.com/abhishri_designs">
                                                              <img src="https://www.google.co.in/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png" width="70" border="0" style="vertical-align:middle;">
                                                              </a>
                                                          </td>
                                                      </tr>
                                                      <tr>
                                                          <td align="center" style="font-size:18px;color:#0e0e0f;font-weight:700;font-family:Helvetica Neue;line-height:28px;vertical-align:top;text-align:center;padding:35px 40px 0px 40px;">
                                                              <strong>You have received an Enquiry from '.$new_arr['person_name'].'</strong>
                                                          </td>
                                                      </tr>
                                                      <tr>
                                                          <td align="center" bgcolor="#ffffff" height="1" style="padding:40px 40px 5px;" valign="top" width="100%">
                                                              <table cellpadding="0" cellspacing="0" width="100%">
                                                                  <tbody>
                                                                      <tr>
                                                                          <td style="border-top:1px solid #e4e4e4;"> </td>
                                                                      </tr>
                                                                  </tbody>
                                                              </table>
                                                          </td>
                                                      </tr>
                                                      <tr style="text-align: justify;">
                                                          <td class="yiv3733200406content" style="font:16px/22px; text-align:left;color:#555555;padding:40px 40px 0px 40px; width: 80%; display: block;">
                                                              <div style="width: 80%; margin: 0 auto;">Hi,<br>'.$new_arr['person_name'].' left you an enquiry in regards to the general:</div>
                                                              <table style="padding-bottom:30px;width:100%;">
                                                                  <tbody>
                                                                      <tr>
                                                                          <td align="center" bgcolor="#ffffff" height="1" style="padding:20px 40px 5px;" valign="top" width="100%">
                                                                              <table cellpadding="0" cellspacing="0" width="100%">
                                                                                  <tbody>
                                                                                      <tr>
                                                                                          <td style="border-top:1px solid #e4e4e4;"> </td>
                                                                                      </tr>
                                                                                  </tbody>
                                                                              </table>
                                                                          </td>
                                                                      </tr>
                                                                      <tr>
                                                                          <td class="yiv3733200406content" style="padding:20px 0px 0px 40px;">
                                                                              Subject: General Enquiry

                                                                          </td>
                                                                      </tr>
                                                                      <tr>
                                                                          <td class="yiv3733200406content" style="text-align: justify; width: 80%; margin: 0 auto;     padding: 20px 0px 0px 0px;display: block;">
                                                                              Hi, I wanted to knwo the best price for dress with product code AD4567890 whose online price is 5500 INR. Please let me know ASAP at my email address xyz@gmail.com.
                                                                              '.$new_arr['description'].'
                                                                          </td>
                                                                      </tr>
                                                                      <tr>
                                                                          <td align="center" bgcolor="#ffffff" height="1" style="padding:20px 40px 5px;" valign="top" width="100%">
                                                                              <table cellpadding="0" cellspacing="0" width="100%">
                                                                                  <tbody>
                                                                                      <tr>
                                                                                          <td style="border-top:1px solid #e4e4e4;"> </td>
                                                                                      </tr>
                                                                                  </tbody>
                                                                              </table>
                                                                          </td>
                                                                      </tr>
                                                                  </tbody>
                                                              </table>
                                                              <table width="100%" border="0" cellspacing="0" cellpadding="0" style="margin:0px 0px 30px 0px;">
                                                                  <tbody>
                                                                      <tr>
                                                                          <td class="yiv3733200406content" style="padding:20px 0px 40px 0px; text-align: center;">
                                                                              His contact details are:<br> '.$new_arr['email_id'].'/'.$new_arr['contact_no'].' 

                                                                          </td>
                                                                      </tr>
                                                                      <tr>
                                                                          <td align="center" style="text-align:center;">
                                                                              <a rel="nofollow" name="CTA" style="color:#ffffff;background-color:#01b22e;display:inline-block;font-family:Helvetica Neue;font-size:16px;line-height:30px;text-align:center;font-weight:bold;text-decoration:none;padding:5px 20px;border-radius:3px;text-transform:none;" target="_blank" href="#">Go To Panel</a>
                                                                          </td>
                                                                      </tr>
                                                                  </tbody>
                                                              </table>
                                                          </td>
                                                      </tr>
                                                  </tbody>
                                              </table>
                                          </td>
                                      </tr>
                                      <tr>
                                          <td width="100%" align="center" valign="top" bgcolor="#ffffff" height="45"></td>
                                      </tr>
                                  </tbody>
                              </table>
                          </td>
                      </tr>
                      <tr>
                          <td bgcolor="#eeeeee" align="center" style="padding:20px 0px;">

                          </td>
                      </tr>
                  </tbody>
              </table>
              </td>
              </tr>
              </tbody></table>
          </body>
      </html>
      ';

      // Always set content-type when sending HTML email
      $headers = "MIME-Version: 1.0" . "\r\n";
      $headers .= "Content-type:text/html;charset=UTF-8" . "\r\n";

      // More headers
      $headers .= 'From: <hr@digitaserve.com>' . "\r\n";

      mail($to,$subject,$message,$headers);


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