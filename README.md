# Islandora Change Content Model

## Introduction

This module provides the ability for a sufficiently privileged user to change an object's content model, retaining all of its existing properties and datastreams. The only thing that changes is the 'hasModel' relationship in the object's RELS-EXT datastream. Using this module to change a content model does not validate the existence of datastreams required by the new content model, does not remove any datastreams from the object, and does not check to see if the new content model is allowed in the object's parent collection. _The user is responsible for all these things_, although third-party modules can implement hooks to do this if they wish.

Even though the effects of changing an object's content model are usually minor (but see the note about collection and compound objects below), changing a content model should be an uncommon event, only necessary when deleting an object and replacing it with one of a different content model is suboptimal. Use this module with caution.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Installation

Same as any Drupal module.

## Configuration

You will need to select which content models objects must have in order to be eligible to have their models changed, at `admin/islandora/tools/change_cmodel`.

In order to access the user interface for changing an object's content model, users must have the 'Change Islandora Content Models' permission provided by this module, plus the following permissions provided by the Islandora module:

* Manage object properties
* Add datastreams to repository objects
* Regenerate derivatives
* Replace datastreams

## Usage

Sufficiently privileged users will see a fourth button, "Change content model", in each object's "Properties" subtab:

![Change content model button](images/change_cmodel_button.png)

Clicking on this button will reveal a form where the user chooses the new content model. If successful, the user is presented with a message saying so and telling them what to do next, e.g.:

> Content model changed to _islandora:sp_large_image_cmodel_. You should replace this object's OJB datastream, if necessary, and regenerate its derivatives.

The standard workflow for using this module is:

1. Use the UI to change an object's content model as described above.
1. If necessary, replace the OBJ with a file that is consistent with the new content model. This is an important step since the OBJ determines what derivatives are regenerated.
1. Regenerate derivatives for the object.
1. If desired, delete any unwanted datastreams.

Note that if you change the content model of an object that has children or members (for example, objects with compound or collection content models), the children/member objects are not affected. Changing the content model of their parent back to what it was will recreate the collection browse, compound display, etc. In general, an object's content model can be changed back to its original with no severe side effects, but if you regenerate derivatives after changing an object's content model, you will need to manually replace the OBJ and regenerate derivatives again.

## Extending/customizing this module

See the `islandora_change_cmodel.api.php` file for hooks this module defines.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Use cases, suggestions, and bug reports, as well as pull requests, are welcome. Please see CONTRIBUTING.md for more information.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
