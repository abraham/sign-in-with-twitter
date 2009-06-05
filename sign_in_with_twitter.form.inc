<?php

/**
 * Implementation of hook_confirm().
 */
function sign_in_with_twitter_confirm_remove(&$form_state) {
  global $user;
  $tid = sign_in_with_twitter_user_get_uid_authmaps($user->uid);
  $form = array();
  $form['#tid'] = $tid;
  _sign_in_with_twitter_screen_name_js($tid);
  return confirm_form(
    $form,
    t('Are you sure you want to break the connection with the Twitter account: <span class="sign-in-with-twitter-id">%tid</span>?', array('%tid' => $tid)),
    'user/'. $user->uid .'/edit',
    t('If you do not have an alternative method of authentication you will not be able to log in again.'),
    t('Confirm'),
    t('Cancel'),
    'sign_in_with_twitter_confirm_remove');
}

/**
 * Implementation of hook_submit().
 */
function sign_in_with_twitter_confirm_remove_submit($form, &$form_state) {
  global $user;
  $tid = $form['#tid'];
  if (_sign_in_with_twitter_remove_connection($tid)) {
    drupal_set_message('notice', 'Twitter connection successfully broken.');
  }
  else {
    drupal_set_message('warning', 'Something may have gone wrong. Try breaking connection with Twitter again.');
  }
  $form_state['redirect'] = "user/{$user->uid}/edit"; 
}