<?php

$plugin = array(
  'title' => 'SQL Server',
  'schema' => 'geofield_sqlsrv_backend_schema',
  'save' => 'geofield_sqlsrv_backend_save',
  'types' => array('Point','LineString','Polygon','MultiPoint','MultiLineString','MultiPolygon','GeometryCollection'),
  'validate' => NULL,     //@@TODO: Additional validation callback. Takes a GeoPHP Geometry object.
  'update_field' => NULL,
  'delete_field' => NULL,  // If we need to get rid of any additional data not on the field table.
  'requirements' => 'geofield_sqlsrv_backend_requirements',
  'settings' => 'geofield_sqlsrv_backend_settings',
);

function geofield_postgis_backend_schema($field) {
  //@@TODO: SRID
  $settings = $field['settings']['sqlsrv'];
  return array(
    'type' => 'blob',
    'sqlsrv_type' => $settings['fieldtype'],
    'not null' => FALSE,
  );
}

function geofield_sqlsrv_backend_save($geometry) {
  $geometry = geoPHP::load($geometry);
  return $geometry->out('wkt');
}

function geofield_sqlsrv_backend_requirements(&$error) {
  $db_type_name = Database::getConnection()->databaseType();
  if ($db_type_name == 'sqlsrv') {
    return TRUE;
  }
  else {
    $error = 'Requires SQL Server (not ' . $db_type_name . ')';
    return FALSE;
  }
}

function geofield_sqlsrv_backend_settings($field, $instance, $has_data) {
  $form = array();
  $defaults = (isset($field['settings']['sqlsrv'])) ? $field['settings']['sqlsrv'] : array();
  
  $form['fieldtype'] = array(
    '#title' => 'SQL Server Type',
    '#type' => 'select',
    '#options' => array(
      'geometry' => 'Geometry',
      'geography' => 'Geography',
    ),
    '#default_value' => isset($defaults['fieldtype']) ? $defaults['fieldtype'] : 'geometry',
    '#disabled' => $has_data,
  );
  
  return $form;
}
