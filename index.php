<?php

/**
 * @file
 * The PHP page that serves all page requests on a Drupal installation.
 *
 * The routines here dispatch control to the appropriate handler, which then
 * prints the appropriate page.
 *
 * All Drupal code is released under the GNU General Public License.
 * See COPYRIGHT.txt and LICENSE.txt.
 */

/**
 * Root directory of Drupal installation.
 */
define('DRUPAL_ROOT', getcwd());

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
menu_execute_active_handler();

// require('includes/password.inc');
// echo user_hash_password('admin@123');
// die;

// define('DRUPAL_ROOT', getcwd());
// require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
// drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
// global $user;
// // Create some data.
// $data = array();
// for ($x = 0; $x < 100; $x++) {
//   $user->data[] = md5($x);
// }
// // Store in session.
// $user->data = array();
// // Commit the session.
// drupal_session_commit();
// // Regenerate the session.
// drupal_session_regenerate();
// // Clear session, wincache has a special 
// // drupal_session_destroy implementation.
// if (function_exists('drupal_session_destroy')) {
//   drupal_session_destroy();
// }
// else {
//   session_destroy();
// }
// // Now create and store a session without destroy
// // so that they accumulate throughout the test.
// $user->data = array();
// // Commit the session.
// drupal_session_commit();
