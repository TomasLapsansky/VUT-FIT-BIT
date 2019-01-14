let ctx = {
	'bspline': {
		coords: {x: 0, y: 0},
		points: [],
		grabbedPoint: null,
		referencePoints: [{x: 317, y: 308}, {x: 380, y: 318}, {x: 386, y: 254}, {x: 457, y: 296}],
		context: null,
		canvas: null,
		id_canvas: 'v_canvas1',
		id_canvas_wrapper: 'v_canvas_wrapper1',
		id_canvas_img: 'v_img1',
		win_trashhold: 0.095,
		pointId: 0,
	},
	'nurbs': {
		coords: {x: 0, y: 0},
		points: [],
		grabbedPoint: null,
		referencePoints: [{x: 647, y: 233, weight: 0}, {x: 761, y: 238, weight: 12},
						{x: 463, y: 345, weight: 51}, {x: 841, y: 338, weight: 0}],
		context: null,
		canvas: null,
		id_canvas: 'v_canvas3',
		id_canvas_wrapper: 'v_canvas_wrapper3',
		id_canvas_img: 'v_img3',
		win_trashhold: 0.095,
		pointId: 0,
	},
	'ferg': {
		coords: {x: 0, y: 0},
		points: [],
		grabbedPoint: null,
		referencePoints: [{x: 306, y: 376}, {x: 352, y: 389}, {x: 391, y: 393}, {x: 439, y: 353}],
		context: null,
		canvas: null,
		id_canvas: 'v_canvas2',
		id_canvas_wrapper: 'v_canvas_wrapper2',
		id_canvas_img: 'v_img2',
		win_trashhold: 0.15,
		pointId: 0,
	},
};

let tmp = initCanvas('bspline');
ctx['bspline'].context = tmp.context;
ctx['bspline'].canvas = tmp.canvas;
tmp = initCanvas('nurbs');
ctx['nurbs'].context = tmp.context;
ctx['nurbs'].canvas = tmp.canvas;
tmp = initCanvas('ferg');
ctx['ferg'].context = tmp.context;
ctx['ferg'].canvas = tmp.canvas;

function initCanvas(curve) {
	let canvas = document.getElementById(ctx[curve].id_canvas);
	canvas.width  = 900;
	canvas.height = 570;

	canvas.addEventListener('mousemove', function(evt) {
		ctx[curve].coords = getMousePos(canvas, evt);
		handleMouseMove(curve);
	}, false);
	canvas.addEventListener('mousedown', function(evt) {
		ctx[curve].coords = getMousePos(canvas, evt);
		handleClick(curve);
	}, false);
	canvas.addEventListener('mouseup', function() {
		ctx[curve].grabbedPoint = null;
		handleMouseMove(curve);
	}, false);
	canvas.addEventListener('DOMMouseScroll', function(evt) {
		handleMouseWheel(evt, curve);
	}, false);
	
	let context = canvas.getContext('2d');
	return {canvas, context};
}

function getMousePos(canvas, evt) {
	var rect = canvas.getBoundingClientRect();
	return {
		x: evt.clientX - rect.left,
		y: evt.clientY - rect.top
	};
}

function handleClick(curve) {
	ctx[curve].grabbedPoint = getHoveredPoint(curve, 15);
	if (ctx[curve].grabbedPoint) {
		document.getElementById(ctx[curve].id_canvas_wrapper).style.cursor = 'grabbing';
		return;
	}

	if (ctx[curve].points.length >= 4) {
		return;
	}
	ctx[curve].points.push({id: ctx[curve].pointId, weight: 0, ...ctx[curve].coords});
	ctx[curve].pointId++;

	render(curve);
}

function handleMouseMove(curve) {
	const hoveredPoint = getHoveredPoint(curve, 15);
	let canvas_wrapper = document.getElementById(ctx[curve].id_canvas_wrapper);
	if (ctx[curve].grabbedPoint) {
		ctx[curve].grabbedPoint.x = ctx[curve].coords.x;
		ctx[curve].grabbedPoint.y = ctx[curve].coords.y;
		canvas_wrapper.style.cursor = 'grabbing';
	}
	else if (hoveredPoint) {
		if (curve === 'nurbs' && hoveredPoint.id == 1 || curve === 'nurbs' && hoveredPoint.id == 2) {
			canvas_wrapper.style.cursor = 'ns-resize';
		} else {
			canvas_wrapper.style.cursor = 'grab';
		}
	} else {
		canvas_wrapper.style.cursor = 'crosshair';
	}
	render(curve);
}

function handleMouseWheel(evt, curve) {
	if (curve !== 'nurbs') {
		return;
	}
	let hoveredPoint = getHoveredPoint(curve, 35);
	if (hoveredPoint) {
		if (hoveredPoint.id != 1 && hoveredPoint.id != 2) {
			return;
		}
		evt.preventDefault(evt.detail);
		hoveredPoint = getHoveredPoint(curve, 15);
		if (!hoveredPoint) {
			return;
		}
		const delta = Math.max(-1, Math.min(1, (evt.wheelDelta || -evt.detail)));
		hoveredPoint.weight += delta*3;
		hoveredPoint.weight = Math.max(0, hoveredPoint.weight);
		hoveredPoint.weight = Math.min(200, hoveredPoint.weight);
		render(curve);
	}
}

function render(curve) {
	let winDistance = getWinDistance(curve);
	if (winDistance < ctx[curve].win_trashhold) {
		console.log('win')
		winDistance = 0;
	} else {
		// console.log(winDistance);
	}
	const opacity = 70 + 30 * (1 - winDistance);
	const grayscale = 100 * winDistance;
	document.getElementById(ctx[curve].id_canvas_img).style.filter
		= 'opacity('+opacity+'%) grayscale('+grayscale+'%)';
	ctx[curve].context.clearRect(0, 0, ctx[curve].canvas.width - 1, ctx[curve].canvas.height - 1);
	if (curve === 'bspline') {
		drawBspline(curve);
	} else if (curve === 'ferg') {
		drawFerguson(curve);
	} else {
		drawNurbs(curve);
	}
	drawPoints(curve);
}

function drawBspline(curve) {
	if (ctx[curve].points.length < 4) {
		return;
	}
	ctx[curve].context.beginPath();
	ctx[curve].context.lineWidth = 2;
	ctx[curve].context.strokeStyle = '#B00000';
	ctx[curve].context.moveTo(ctx[curve].points[0].x, ctx[curve].points[0].y);
	ctx[curve].context.bezierCurveTo(	ctx[curve].points[1].x, ctx[curve].points[1].y,
										ctx[curve].points[2].x, ctx[curve].points[2].y,
										ctx[curve].points[3].x, ctx[curve].points[3].y);
	ctx[curve].context.stroke();
	ctx[curve].context.beginPath();
	ctx[curve].context.lineWidth = 1;
	ctx[curve].context.strokeStyle = '#00DBDB';
	ctx[curve].context.moveTo(ctx[curve].points[0].x, ctx[curve].points[0].y);
	ctx[curve].context.lineTo(ctx[curve].points[1].x, ctx[curve].points[1].y);
	ctx[curve].context.lineTo(ctx[curve].points[2].x, ctx[curve].points[2].y);
	ctx[curve].context.lineTo(ctx[curve].points[3].x, ctx[curve].points[3].y);
	ctx[curve].context.stroke();
}

function drawNurbs(curve) {
	if (ctx[curve].points.length < 4) {
		return;
	}
	let v01 = normalizeVector({
		x: ctx[curve].points[1].x - ctx[curve].points[0].x,
		y: ctx[curve].points[1].y - ctx[curve].points[0].y,
	});
	let v21 = normalizeVector({
		x: ctx[curve].points[1].x - ctx[curve].points[2].x,
		y: ctx[curve].points[1].y - ctx[curve].points[2].y,
	});
	let v32 = normalizeVector({
		x: ctx[curve].points[2].x - ctx[curve].points[3].x,
		y: ctx[curve].points[2].y - ctx[curve].points[3].y,
	});
	let vw1 = normalizeVector({
		x: v01.x + v21.x,
		y: v01.y + v21.y,
	});
	let vw2 = normalizeVector({
		x: v32.x - v21.x,
		y: v32.y - v21.y,
	});
	let pw1 = {
		x: ctx[curve].points[1].x + ctx[curve].points[1].weight * vw1.x,
		y: ctx[curve].points[1].y + ctx[curve].points[1].weight * vw1.y,
	};
	let pw2 = {
		x: ctx[curve].points[2].x + ctx[curve].points[2].weight * vw2.x,
		y: ctx[curve].points[2].y + ctx[curve].points[2].weight * vw2.y,
	};
	let middlePoint = {
		x: (pw1.x + pw2.x) / 2,
		y: (pw1.y + pw2.y) / 2,
	};
	ctx[curve].context.beginPath();
	ctx[curve].context.lineWidth = 2;
	ctx[curve].context.strokeStyle = '#B00000';
	ctx[curve].context.moveTo(ctx[curve].points[0].x, ctx[curve].points[0].y);
	ctx[curve].context.quadraticCurveTo(pw1.x, pw1.y, middlePoint.x, middlePoint.y);
	ctx[curve].context.moveTo(middlePoint.x, middlePoint.y);
	ctx[curve].context.quadraticCurveTo(pw2.x, pw2.y, ctx[curve].points[3].x, ctx[curve].points[3].y);
	ctx[curve].context.stroke();
	ctx[curve].context.beginPath();
	ctx[curve].context.setLineDash([5, 3]);
	ctx[curve].context.lineWidth = 1;
	ctx[curve].context.strokeStyle = '#001EFF';
	ctx[curve].context.moveTo(ctx[curve].points[0].x, ctx[curve].points[0].y);
	ctx[curve].context.lineTo(ctx[curve].points[1].x, ctx[curve].points[1].y);
	ctx[curve].context.lineTo(ctx[curve].points[2].x, ctx[curve].points[2].y);
	ctx[curve].context.lineTo(ctx[curve].points[3].x, ctx[curve].points[3].y);
	ctx[curve].context.stroke();
	ctx[curve].context.setLineDash([]);
}

function drawFerguson(curve) {
	if (ctx[curve].points.length < 4) {
		return;
	}

	if (isConvex(curve)) {
		ctx[curve].context.beginPath();
		ctx[curve].context.lineWidth = 2;
		ctx[curve].context.strokeStyle = '#B00000';
		ctx[curve].context.moveTo(ctx[curve].points[0].x, ctx[curve].points[0].y);
		ctx[curve].context.bezierCurveTo(	ctx[curve].points[1].x, ctx[curve].points[1].y,
											ctx[curve].points[2].x, ctx[curve].points[2].y,
											ctx[curve].points[3].x, ctx[curve].points[3].y);
	}
	ctx[curve].context.stroke();
	ctx[curve].context.beginPath();
	ctx[curve].context.lineWidth = 1;
	ctx[curve].context.strokeStyle = '#98FF1C';
	ctx[curve].context.moveTo(ctx[curve].points[0].x, ctx[curve].points[0].y);
	ctx[curve].context.lineTo(ctx[curve].points[1].x, ctx[curve].points[1].y);
	ctx[curve].context.moveTo(ctx[curve].points[3].x, ctx[curve].points[3].y);
	ctx[curve].context.lineTo(ctx[curve].points[2].x, ctx[curve].points[2].y);
	ctx[curve].context.stroke();
}

function isConvex(curve) {
	let points = ctx[curve].points;
	if (points.length < 4) {
		return true;
	}
	let sign = false;
	for (i = 0; i < points.length; i++) {
		let v1 = {
			x: points[(i+2) % points.length].x - points[(i+1) % points.length].x,
			y: points[(i+2) % points.length].y - points[(i+1) % points.length].y,
		};
		let v2 = {
			x: points[i].x - points[(i+1) % points.length].x,
			y: points[i].y - points[(i+1) % points.length].y,
		};
		let crossProduct = v1.x * v2.y - v1.y * v2.x;
		if (i == 0) {
			sign = (crossProduct >= 0);
		} else if (sign != (crossProduct > 0)) {
			return false;
		}
	}
	return true;
}

function drawPoints(curve) {
	for (i = 0; i < ctx[curve].points.length; i++) {
		ctx[curve].context.beginPath();
		ctx[curve].context.fillStyle = 'black';
		if (ctx[curve].points[i].id == 1 || ctx[curve].points[i].id == 2) {
			ctx[curve].context.fillStyle = '#DB6B6B';
		}
		ctx[curve].context.lineWidth = 1;
		ctx[curve].context.strokeStyle = 'white';
		ctx[curve].context.rect(ctx[curve].points[i].x - 4, ctx[curve].points[i].y - 4, 8, 8);
		ctx[curve].context.fill();
		ctx[curve].context.stroke();
	}
}

function getHoveredPoint(curve, radius) {
	let points = ctx[curve].points;
	const coords = ctx[curve].coords;
	for (i = 0; i < points.length; i++) {
		if (distance(coords, points[i]) <= radius) {
			return points[i];
		}
	}
	return null;
}

function getWinDistance(curve) {
	if (!ctx[curve].points.length || ctx[curve].points.length != ctx[curve].referencePoints.length) {
		return 1;
	}
	if (curve === 'ferg' && !isConvex(curve)) {
		return 1;
	}
	let distSum = 0;
	for (i = 0; i < ctx[curve].points.length; i++) {
		distSum += Math.min(100, distance(ctx[curve].points[i], ctx[curve].referencePoints[i]));
	}
	if (curve === 'nurbs') {
		distSum += Math.abs(ctx[curve].points[1].weight - ctx[curve].referencePoints[1].weight);
		distSum += Math.abs(ctx[curve].points[2].weight - ctx[curve].referencePoints[2].weight);
		distSum = Math.min(ctx[curve].points.length * 100, distSum);
	}

	return distSum / (ctx[curve].points.length * 100);
}

function distance(p1, p2) {
	if (p1.x === p2.x && p1.y === p2.y) {
		return 0;
	}
	return Math.sqrt((p1.x - p2.x)**2 + (p1.y - p2.y)**2);
}

function normalizeVector(v) {
	let d = Math.sqrt(v.x**2 + v.y**2);
	return {x: v.x / d, y: v.y / d};
}