<?php
// $Id$

/**
 * @file
 * Admin settings page for the sign_in_with_twitter module.
 */

/**
 * Implementation of hook_admin().
 */
function sign_in_with_twitter_admin() {
  $module_path = drupal_get_path('module', 'sign_in_with_twitter') .'/images/';
  global $base_url;
  $form = array();
  $form['sign_in_with_twitter_callback'] = array(
    '#type' => 'item',
    //'#readonly' => TRUE,
    '#title' => t('Callback URL'),
    '#value' => $base_url.'/sign-in-with-twitter/callback',
    '#description' => t('Callback URL for the application settings on <a href="https://twitter.com/oauth_clients">https://twitter.com/oauth_clients</a>.'),
  );
  $form['sign_in_with_twitter_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer key'),
    '#default_value' => variable_get('sign_in_with_twitter_consumer_key', ''),
    '#description' => t('Consumer key from your application settings page.'),
  );

  $form['sign_in_with_twitter_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer secret'),
    '#default_value' => variable_get('sign_in_with_twitter_consumer_secret', ''),
    '#description' => t('Consumer secret from your application settings page.'),
  );
  $form['sign_in_with_twitter_button'] = array(
    '#type' => 'radios',
    '#title' => t('Select sign in button'),
    '#default_value' => variable_get('sign_in_with_twitter_button', 'Sign-in-with-Twitter-darker.png'),
    '#description' => t('Select the button to display to anonymous users.'),
    '#options' => array(
      'Sign-in-with-Twitter-darker.png' => theme_image($module_path .'Sign-in-with-Twitter-darker.png'),
      'Sign-in-with-Twitter-darker-small.png' => theme_image($module_path .'Sign-in-with-Twitter-darker-small.png'),
      'Sign-in-with-Twitter-lighter.png' => theme_image($module_path .'Sign-in-with-Twitter-lighter.png'),
      'Sign-in-with-Twitter-lighter-small.png' => theme_image($module_path .'Sign-in-with-Twitter-lighter-small.png')
    ),
  );
  return system_settings_form($form);
} // function sign_in_with_twitter_admin().