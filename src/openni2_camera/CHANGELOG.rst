^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package openni2_camera
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

0.2.4 (2015-04-06)
------------------
* proper usage of device_id parameter in resolveDeviceURI, resolve unique parts of device URIs, too
* print vendor id and product id as hex value (like in lsusb)
* Contributors: Michael Ferguson, Stephan Wirth

0.2.3 (2015-01-16)
------------------
* Explicitly ask for serial number when trying to resolve device URI instead of doing it once on device connected, fixes `#24 <https://github.com/ros-drivers/openni2_camera/issues/24>`_
* Contributors: Michael Ferguson, Stephan Wirth

0.2.2 (2014-10-06)
------------------
* Add usb_reset
* Contributors: Kei Okada, Michael Ferguson

0.2.1 (2014-08-22)
------------------
* Fixed a bug that prevents depth only sensors from properly calculating the point cloud due to incorrect focal length
* Updated cmakelists for OSX
* Contributors: Colin Lea, Michael Ferguson, Tarek Taha

0.2.0 (2014-05-22)
------------------
* device_id: find camera by serial number
* Make freenect_stack link a real link for wiki.
* Contributors: Dariush Forouher, Michael Ferguson

0.1.2 (2014-02-03)
------------------
* Fix CMake error.
* Contributors: Benjamin Chretien, Michael Ferguson

0.1.1 (2013-11-13)
------------------
* Fixed default value of ir_mode. Thanks @nxdefiant
  https://github.com/ros-drivers/openni2_camera/issues/16

0.1.0 (2013-08-28)
------------------
* initial release
