<?php
// $Id$

/**
 * @file
 * Include file for Sign in wit Twitter module.
 */
  module_load_include('php', 'sign_in_with_twitter', 'OAuth');

/**
 * Callback to get OAuth request token from Twitter and redirect user to start authentication process.
 */
function sign_in_with_twitter_redirect() {
  module_load_include('module', 'sign_in_with_twitter', 'OAuth.php');
  parse_str(_sign_in_with_twitter_make_api_call('https://twitter.com/oauth/request_token'));
  $_SESSION['sign_in_with_twitter_oauth_token'] = $oauth_token;
  $_SESSION['sign_in_with_twitter_oauth_token_secret'] = $oauth_token_secret;
  drupal_goto('https://twitter.com/oauth/authenticate?oauth_token='. $oauth_token);
} // sign_in_with_twitter_redirect().

/**
 * User returns from Twitter after authenticating and access tokens are processed.
 */
function sign_in_with_twitter_callback() {
  global $user;
  if (empty($_GET['oauth_token']) || $_GET['oauth_token'] != $_SESSION['sign_in_with_twitter_oauth_token']) {
    drupal_set_message('Something went wrong. Please try again.', 'warning');
    drupal_goto('<front>');
  }
  $request_token = new OAuthToken($_GET['oauth_token'], $_SESSION['sign_in_with_twitter_oauth_token_secret']);
  parse_str(_sign_in_with_twitter_make_api_call('https://twitter.com/oauth/access_token', $request_token));
  $_SESSION['sign_in_with_twitter_oauth_token'] = $oauth_token;
  $_SESSION['sign_in_with_twitter_oauth_token_secret'] = $oauth_token_secret;
  $user_token = new OAuthConsumer($oauth_token, $oauth_token_secret);
  $twitter_user = json_decode(_sign_in_with_twitter_make_api_call('https://twitter.com/account/verify_credentials.json', $user_token));
  $external_id = user_external_load($twitter_user->id);

  // User is already logged in.
  if (!empty($user->uid)) {
    if (!empty($external_id)) {
      // tid already attached.
      if ($external_id->uid == $user->uid) {
        drupal_set_message('You have already connected with that Twitter account.', 'warning');
      }
      else {
        drupal_set_message('That Twitter user is already connected with an account.', 'warning');
      }
      drupal_goto('user/'. $user->uid);
    }
    else {
      // Attach tid to user
      user_set_authmaps($user, array('authmap_sign_in_with_twitter' => $twitter_user->id));
      drupal_set_message('Twitter account connected.', 'notice');
      drupal_goto('user/'. $user->uid);
    }
  }// User is already logged in.
  // User is Anon.
  else {
    if (!empty($external_id)) {
      // log user in
      user_external_login(user_external_load($twitter_user->id));
      drupal_goto('user/'. $user->uid);
    }
    else {
      // register account.
      $_SESSION['sign_in_with_twitter_status'] = 'authenticated';
      $_SESSION['sign_in_with_twitter_user'] = $twitter_user;
      drupal_goto('user/register');
    }
  } // User is Anon.
} // sign_in_with_twitter_callback().

/**
 * Create OAuthConsumer object.
 */
function _sign_in_with_twitter_create_oauthconsumer() {
  return new OAuthConsumer(
    variable_get('sign_in_with_twitter_consumer_key', FALSE),
    variable_get('sign_in_with_twitter_consumer_secret', FALSE)
  );
} // _sign_in_with_twitter_create_oauthconsumer().

/**
 * Create OAuth object and onnect to the Twitter api.
 */
function _sign_in_with_twitter_make_api_call($api_path, $token = NULL, $goto = '<front>') {
  $hmac_method = new OAuthSignatureMethod_HMAC_SHA1();
  $consumer = _sign_in_with_twitter_create_oauthconsumer();
  $req_req = OAuthRequest::from_consumer_and_token($consumer, $token, "GET", $api_path);
  $req_req->sign_request($hmac_method, $consumer, $token);
  $request = drupal_http_request($req_req);
  if (200 != $request->code) {
    drupal_set_message('Could not connect to Twitter. Please try again later.', 'error');
    drupal_goto($goto);
  }
    return $request->data;
}