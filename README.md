# Islandora Change Content Models

> Note: This module is still in early development. Do not use it.

## Introduction

This module provides the ability for a sufficiently privilegded user to change an object's content model, retaining all of its existing properties and datastreams. It does not validate the existence of datastreams required by the new content model, and does not remove any datastreams from the object. The user is responsible to managing the datastreams (although third-party modules can implement hooks to do this if they wish; see below for more information).

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Installation

Same as any Drupal module.

## Configuration

You will need to enable which content models objects must have in order to have their models changed at `admin/islandora/tools/change_cmodel`.

Users must have the 'Change Islandora Content Models' permission provided by this module, plus the following permissions provided by the Islandora module:

* Manage object properties
* Add datastreams to repository objects
* Regenerate derivatives
* Replace datastreams


## Usage


## Extending/customizing this module

See the islandora_change_cmodel.api.php file for hooks this module defines.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Use cases, suggestions, and bug reports, as well as pull requests, are welcome. Please see CONTRIBUTING.md for more information.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
