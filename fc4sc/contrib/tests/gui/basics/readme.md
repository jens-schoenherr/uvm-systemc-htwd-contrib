Comparison of gui output
========================

There is no known way to compare the browser output without manual interaction.
To get the current browser output in a machine readable format:
1. run `contrib/tests/gui/test_gui.sh` or `contrib/tests/gui/basics/build/gui_test_basics` to create `.xml` files
2. open `contrib/tools/gui/index.html` in a HTML browser (e.g. firefox)
3. load the `.xml` file (e.g. `char_cvp_coverage.xml`)
4. open in firefox the "Web Developer Tools" (under "More Tools" or press Ctrl-Shift-I)
5. select the opening `<html>` tag and save the html content to a `.xml.html` file (e.g. `char_cvp_coverage.xml.html`)
6. repeat steps 2..5 for all `.xml` files
7. run `contrib/tests/gui/test_gui.sh` again to compare all created `.xml.html` files


