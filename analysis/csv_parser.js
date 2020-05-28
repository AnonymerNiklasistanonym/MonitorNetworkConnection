const CSV = {
	parse: function(csv, reviver) {
		reviver = reviver || function(r, c, v) { return v; };
		var chars = csv.split(''), c = 0, cc = chars.length, start, end, table = [], row;
		while (c < cc) {
			table.push(row = []);
			while (c < cc && '\r' !== chars[c] && '\n' !== chars[c]) {
				start = end = c;
				if ('"' === chars[c]){
					start = end = ++c;
					while (c < cc) {
						if ('"' === chars[c]) {
							if ('"' !== chars[c+1]) { break; }
							else { chars[++c] = ''; } // unescape ""
						}
						end = ++c;
					}
					if ('"' === chars[c]) { ++c; }
					while (c < cc && '\r' !== chars[c] && '\n' !== chars[c] && ',' !== chars[c]) { ++c; }
				} else {
					while (c < cc && '\r' !== chars[c] && '\n' !== chars[c] && ',' !== chars[c]) { end = ++c; }
				}
				row.push(reviver(table.length-1, row.length, chars.slice(start, end).join('')));
				if (',' === chars[c]) { ++c; }
			}
			if ('\r' === chars[c]) { ++c; }
			if ('\n' === chars[c]) { ++c; }
		}
		return table;
	},

	stringify: function(table, replacer) {
		replacer = replacer || function(r, c, v) { return v; };
		var csv = '', c, cc, r, rr = table.length, cell;
		for (r = 0; r < rr; ++r) {
			if (r) { csv += '\r\n'; }
			for (c = 0, cc = table[r].length; c < cc; ++c) {
				if (c) { csv += ','; }
				cell = replacer(r, c, table[r][c]);
				if (/[,\r\n"]/.test(cell)) { cell = '"' + cell.replace(/"/g, '""') + '"'; }
				csv += (cell || 0 === cell) ? cell : '';
			}
		}
		return csv;
	}
};

const convert2Object = (csvArray) => {
	let result = {}
	const header = csvArray[0]
	for (const headerElement of header) {
		result[headerElement] = []
	}
	for (const dataElement of csvArray.slice(1)) {
		for (let i = 0; i < header.length; i++) {
			result[header[i]].push(dataElement[i])
		}
	}
	return result;
};

const unique = (value, index, self) => {
	return self.indexOf(value) === index
}

const convert2UrlDatasets = (csvObject) => {
	let result = {}
	const keys = Object.keys(csvObject)
	// del keys.URL
	const uniqueUrls = csvObject.URL.filter(unique);
	for (const uniqueUrl of uniqueUrls) {
		result[uniqueUrl] = {}
		for (const key of keys) {
			result[uniqueUrl][key] = []
		}
	}
	for (let i = 0; i < csvObject.URL.length; i++) {
		for (let j = 0; j < keys.length; j++) {
			result[csvObject.URL[i]][keys[j]].push(csvObject[keys[j]][i])
		}
	}
	return result;
};

const parseCsvData = (csvDataString) => {
	const parsedCsvData = CSV.parse(csvDataString)
	return convert2UrlDatasets(convert2Object(parsedCsvData))
}
