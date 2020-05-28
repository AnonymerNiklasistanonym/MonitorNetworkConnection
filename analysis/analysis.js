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


	let labels = []
	for (const url in data) {
		labels = data[url].DATE_ISO.map(a => new Date(a))
	}
	const datasets = []
	for (const url in data) {
		const colorName = colorNames[datasets.length % colorNames.length];
		const newColor = chartColors[colorName];
		const datasetData = []
		const dataSpeedAvg = data[url].CONNECTION_SPEED_MS_AVG.map(a => Number(a))
		const dataDates = data[url].DATE_ISO.map(a => new Date(a))
		for (let i = 0; i < dataDates.length; i++) {
			datasetData.push({
				x: moment(dataDates[i]),
				y: dataSpeedAvg[i] == -1 ? -100 : dataSpeedAvg[i]
			})
		}
		datasets.push({
			label: url,
			backgroundColor: color(newColor).alpha(0.5).rgbString(),
			borderColor: newColor,
			fill: false,
			data: datasetData /*: data[url].CONNECTION_SPEED_MS_AVG.map(a => Number(a))*/
		})
	}

	var timeFormat = 'MM/DD/YYYY';

	function newDate(days) {
		return moment().add(days, 'd').toDate();
	}

	function newDateString(days) {
		return moment().add(days, 'd').format(timeFormat);
	}


	var config = {
		type: 'line',
		data: {
			labels /*[ // Date Objects
				newDate(0),
				newDate(1),
				newDate(2),
				newDate(3),
				newDate(4),
				newDate(5),
				newDate(6)
			]*/,
			datasets /*: [{
				label: 'My First dataset',
				backgroundColor: color(chartColors.red).alpha(0.5).rgbString(),
				borderColor: chartColors.red,
				fill: false,
				data: [
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor()
				],
			}, {
				label: 'My Second dataset',
				backgroundColor: color(chartColors.blue).alpha(0.5).rgbString(),
				borderColor: chartColors.blue,
				fill: false,
				data: [
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor(),
					randomScalingFactor()
				],
			}, {
				label: 'Dataset with point data',
				backgroundColor: color(chartColors.green).alpha(0.5).rgbString(),
				borderColor: chartColors.green,
				fill: false,
				data: [{
					x: newDateString(0),
					y: randomScalingFactor()
				}, {
					x: newDateString(5),
					y: randomScalingFactor()
				}, {
					x: newDateString(7),
					y: randomScalingFactor()
				}, {
					x: newDateString(15),
					y: randomScalingFactor()
				}],
			}]*/
		},
		options: {
			title: {
				text: 'Chart.js Time Scale'
			},
			scales: {
				xAxes: {
					type: 'time',
					time: {
						format: "HH:mm",
						unit: 'hour',
						unitStepSize: 1,
						displayFormats: {
						  'minute': 'HH:mm',
						  'hour': 'HH:mm',
						  min: '00:00',
						  max: '23:59'
						}
					},
					scaleLabel: {
						display: true,
						labelString: 'Date'
					}
				},
				y: {
					scaleLabel: {
						display: true,
						labelString: 'value'
					}
				}
			},
		}
	};
	console.log(config)

	const ctx = document.getElementById('canvas').getContext('2d');
	const myLine = new Chart(ctx, config);

}, false);
