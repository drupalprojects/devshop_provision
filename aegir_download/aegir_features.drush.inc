<?php

use Symfony\Component\Process\Process;
use GitWrapper\GitWrapper;
use GitWrapper\GitWorkingCopy;
use GitWrapper\GitException;

/**
 * Implementation of hook_drush_command().
 * Provides provision commands for all devshop tasks.
 */
function aegir_features_drush_command()
{
  $items['provision-features_update'] = array(
    'description' => 'Dummy command. Does nothing.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'features' => 'The list of features to update.',
      'revert' => 'Whether or not to revert the features afterwards.',
    ),
  );
  return $items;
}

/**
 * Implements the provision-fua command.
 */
function drush_aegir_features_provision_features_update() {

  if (d()->type == 'site') {

    $environment = (object) d()->environment;

    $features = trim(drush_get_option('features', ''));
    $revert = drush_get_option('revert', FALSE);

    // If features were specififed...
    if (!empty($features)) {
      devshop_drush_process("drush {$environment->drush_alias} fu {$features} -y", NULL, dt('Update Features'));
    }
    else {
      devshop_drush_process("drush {$environment->drush_alias} fua -y", NULL, dt('Update All Features'));
    }

    // Show git status
    devshop_drush_process("git status", $environment->repo_root, NULL, dt('Checking git status'));

    // Revert force revert the features if the user asked.
    if ($revert) {
      if (!empty($features)) {
        devshop_drush_process("drush {$environment->drush_alias} fr {$features} --force -y", NULL, dt('Revert Features'));
      }
      else {
        devshop_drush_process("drush {$environment->drush_alias} fra --force -y", NULL, t('Revert Features'));
      }
    }
  }
}