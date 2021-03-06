# FBPDF

A PDF viewer for Linux framebuffer and Microwindows Nano-X

```
FBPDF
=====

Fbpdf is a framebuffer pdf and djvu viewer.  There are three make
targets: fbpdf uses mupdf library for rendering pdf, fbpdf2 uses
poppler for the same purpose, and fbdjvu uses djvulibre library for
rendering djvu files.  The following options are available in all
three programs:

  fbpdf [-r rotation] [-z zoom_x10] [-p page_number] file.pdf

The following table lists the commands available in fbpdf.  Most of
them accept a numerical prefix.  For instance, '^F' tells fbpdf to
show the next page while '5^F' tells it to show the fifth next page.

==============	================================================
KEY		ACTION
==============	================================================
^F/J		next page
^B/K		previous page
G		go to page (the last page if no prefix)
o		set page number (for 'G' command only)
O		set page number and go to current page
z		zoom; prefix multiplied by 10 (i.e. '15z' = 150%)
r		set rotation in degrees
i		print some information
I		invert colors
q		quit
^[/escape 	clear the numerical prefix
mx		mark page as 'x' (or any other letter)
'x		jump to the page marked as 'x'
`x		jump to the page and position marked as 'x'
j		scroll down
k		scroll up
h		scroll left
l		scroll right
[		align with the left edge of the page
]		align with the right edge of the page
{		align with the leftmost character on the page
}		align with the rightmost character on the page
H		show page top
M		centre the page vertically
L		show page bottom
C		centre the page horizontally
^D/space	page down
^U/^H/backspace	page up
^L		redraw
e		reload current file
f		zoom to fit page height
w		zoom to fit page width
W		zoom to fit page contents horizontally
Z		set the default zoom level for 'z' command
d		sleep one second before the next command
==============	================================================
```

![screenshot](/fbpdf-nano-X.png)

FBPDF running on Nano-X
