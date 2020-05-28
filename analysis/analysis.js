document.addEventListener('DOMContentLoaded', async () => {


	const data = await fetch("data.csv").then(res => res.text()).then(parseCsvData)

	const chartColors = {
		red: 'rgb(255, 99, 132)',
		orange: 'rgb(255, 159, 64)',
		yellow: 'rgb(255, 205, 86)',
		green: 'rgb(75, 192, 192)',
		blue: 'rgb(54, 162, 235)',
		purple: 'rgb(153, 102, 255)',
		grey: 'rgb(201, 203, 207)'
	};


	const MONTHS = [
		'January',
		'February',
		'March',
		'April',
		'May',
		'June',
		'July',
		'August',
		'September',
		'October',
		'November',
		'December'
	];

	const COLORS = [
		'#4dc9f6',
		'#f67019',
		'#f53794',
		'#537bc4',
		'#acc236',
		'#166a8f',
		'#00a950',
		'#58595b',
		'#8549ba'
	];

	var Samples = Samples || (Samples = {});
	var Color = Chart.helpers.color;

	Samples.utils = {
		// Adapted from http://indiegamr.com/generate-repeatable-random-numbers-in-js/
		srand: function (seed) {
			this._seed = seed;
		},

		rand: function (min, max) {
			var seed = this._seed;
			min = min === undefined ? 0 : min;
			max = max === undefined ? 1 : max;
			this._seed = (seed * 9301 + 49297) % 233280;
			return min + (this._seed / 233280) * (max - min);
		},

		numbers: function (config) {
			var cfg = config || {};
			var min = cfg.min || 0;
			var max = cfg.max || 1;
			var from = cfg.from || [];
			var count = cfg.count || 8;
			var decimals = cfg.decimals || 8;
			var continuity = cfg.continuity || 1;
			var dfactor = Math.pow(10, decimals) || 0;
			var data = [];
			var i, value;

			for (i = 0; i < count; ++i) {
				value = (from[i] || 0) + this.rand(min, max);
				if (this.rand() <= continuity) {
					data.push(Math.round(dfactor * value) / dfactor);
				} else {
					data.push(null);
				}
			}

			return data;
		},

		labels: function (config) {
			var cfg = config || {};
			var min = cfg.min || 0;
			var max = cfg.max || 100;
			var count = cfg.count || 8;
			var step = (max - min) / count;
			var decimals = cfg.decimals || 8;
			var dfactor = Math.pow(10, decimals) || 0;
			var prefix = cfg.prefix || '';
			var values = [];
			var i;

			for (i = min; i < max; i += step) {
				values.push(prefix + Math.round(dfactor * i) / dfactor);
			}

			return values;
		},

		months: function (config) {
			var cfg = config || {};
			var count = cfg.count || 12;
			var section = cfg.section;
			var values = [];
			var i, value;

			for (i = 0; i < count; ++i) {
				value = MONTHS[Math.ceil(i) % 12];
				values.push(value.substring(0, section));
			}

			return values;
		},

		color: function (index) {
			return COLORS[index % COLORS.length];
		},

		transparentize: function (color, opacity) {
			var alpha = opacity === undefined ? 0.5 : 1 - opacity;
			return Color(color).alpha(alpha).rgbString();
		}
	};

	// DEPRECATED
	window.randomScalingFactor = function () {
		return Math.round(Samples.utils.rand(-100, 100));
	};

	// INITIALIZATION

	Samples.utils.srand(Date.now());


	var color = Chart.helpers.color;
	var colorNames = Object.keys(chartColors);


	const minThreshold = 20.0
	const maxThreshold = 90.0

	const skipMax = 40
	let latestDate = undefined
	let oldestDate = undefined
	let maxLabelCount = 0
	const datasets = []
	for (const url in data) {
		const colorName = colorNames[datasets.length % colorNames.length];
		const newColor = chartColors[colorName];
		const datasetData = []
		const dataSpeedAvg = data[url].CONNECTION_SPEED_MS_AVG.map(a => Number(a))
		const dataDates = data[url].DATE_ISO.map(a => new Date(a))
		let skipCounter = 0
		let labelCount = 0
		for (let i = 0; i < dataDates.length; i++) {
			if (oldestDate === undefined || oldestDate > dataDates[i]) {
				oldestDate = moment(dataDates[i])
			}
			if (latestDate === undefined || latestDate < dataDates[i]) {
				latestDate = moment(dataDates[i])
			}
			// Skip elements if
			//if (i > 0 && dataSpeedAvg[i] !== -1 && ++skipCounter < skipMax) {
			//	// Sto
			//	// Skip element if there is nearly a difference
			//	let difference = Math.pow(dataSpeedAvg[i - 1], 2) - Math.pow(dataSpeedAvg[i], 2)
			//	if (dataSpeedAvg[i - 1] !== -1 && (difference > maxThreshold || difference < minThreshold)) {
			//		continue;
			//	}
			//} else {
			//	skipCounter = 0
			//}
			labelCount++
			datasetData.push({
				x: dataDates[i],
				y: dataSpeedAvg[i] == -1 ? -100 : dataSpeedAvg[i]
			})
		}
		if (labelCount > maxLabelCount) {
			maxLabelCount = labelCount
		}
		datasets.push({
			label: url,
			backgroundColor: color(newColor).alpha(0.5).rgbString(),
			borderColor: newColor,
			fill: false,
			data: datasetData /*: data[url].CONNECTION_SPEED_MS_AVG.map(a => Number(a))*/
		})
	}

	let currentDate = oldestDate
	let labels = [ ]
	;
	const minutesDuration = moment.duration(latestDate.diff(oldestDate)).asMinutes();
	const minutesDelta = minutesDuration / maxLabelCount
	while (currentDate < latestDate) {
		labels.push(currentDate.toDate())
		currentDate = currentDate.add(minutesDelta, 'm')
	}
	console.log(labels)

	drawLineGraph(document.getElementById('canvas'), labels, datasets)

}, false);
