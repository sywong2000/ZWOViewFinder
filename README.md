# ZWO ViewFinder 

A Simple Electronic ViewFinder for ZWO cameras.

This program can be used as a Elecrontic Finder for high magnification planetary imaging, using a ASI camera on a guidescope.
The program simply display the full FOV of the guide camera on the left, and allow the user to highlight a ROI (Region Of Interest) and magnify the ROI on the right, with a crosshair for centering the target from the mainscope.

The main purpose for this program is to allow the user to make use of the guidecam with typical guidescope focal length (~120mm) and serve it as a high power magnification viewfinder. 
From time to time I've found using a visual viewfinder (8x or 9x) is kind of difficult when aligning the target (e.g. planets) to the mainscope, during high power planetary imaging. Even the target is at the crosshair already in the viewfinder, the power is not high enough to locate its position for the mainscope. The situation becomes more difficult while moving between targets.

Yet, there's **NO MAGIC**: The user will need to align with a reference object first to ensure the mainscope and guidescope is pointing at the same field of view.



## Reference test setup

### On mainscope
* 8 inch SCT with 2.5x barlow (~5000mm focal length) with ASI 462 camera 

### Guidescope
* 120mm focal length guidescope with ASI 178 camera

The program works perfectly at 50x magnification of the ROI with the above setup.

This is a VIsual Studio 2019 project requiring openCV 4.5.3 and ASI SDK DLL ASICamera2.dll
Sorry I do not have much time to tidy up the codes and put the library locations in this VC project. So I hardcode the paths.



## Steps
1. Mount the guidescope and mainscope. Connect the guiding and capturing cameras.
2. Start the ZWO finder application, select the camera from the dropdown, which is mounted on the _**GUIDESCOPE**_.
3. The panel on the right side will show the full view of the guide camera, on the right side it's a magnifying view of the ROI (white rectangle appearing on the left view).
4. Once you find your target at the main scope, you can highlight the ROI of the target by holding the mouse button on the panel on the left side. Move the ROI rectangle and surround the same target. 
5. You can fine-tune the position of the ROI at the magnified view (right side) by holding the mouse button and move along the mouse.
6. Lock the position of the ROI. Now the ROI Rectangle is aligned with the mainscope FOV.



* You can set the dialog to transparent (button on top right) so that you can view the underlying applications (for my case the capturing program). This is for the convenient when you move your telescope mount and wanted to observe from the viewfinder application and underlying capturing program.
* You can use the "Pause" button to pause the video. This will allow the video capture to pause and reduce the CPU use of this program.


THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
