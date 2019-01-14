$("#bspline_demo").click(function() {
	$([document.documentElement, document.body]).animate({
		scrollTop: $("#slide_1").offset().top
	}, 500);
});

$("#bspline_theory").click(function() {
	$([document.documentElement, document.body]).animate({
		scrollTop: $("#slide_2").offset().top
	}, 500);
});

$("#ferguson_demo").click(function() {
	$([document.documentElement, document.body]).animate({
		scrollTop: $("#slide_3").offset().top
	}, 500);
});

$("#ferguson_theory").click(function() {
	$([document.documentElement, document.body]).animate({
		scrollTop: $("#slide_4").offset().top
	}, 500);
});

$("#nurbs_demo").click(function() {
	$([document.documentElement, document.body]).animate({
		scrollTop: $("#slide_5").offset().top
	}, 500);
});

$("#nurbs_theory").click(function() {
	$([document.documentElement, document.body]).animate({
		scrollTop: $("#slide_6").offset().top
	}, 500);
});

$("#menu").css({opacity: 0.2});
$("#menu").hover(function () {
    $(this).stop(true, false).animate({
        height: "70px",
        opacity: 1
    });
}, function () {
    $(this).stop(true, false).animate({
    	height: "35px",
    	opacity: 0.2
    });
});