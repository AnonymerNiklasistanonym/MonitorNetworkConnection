const drawLineGraph = (canvas, labels, datasets) => {

    var config = {
        type: 'line',
        data: {
            labels,
            datasets
        },
        options: {
            responsive: true,
            title: {
                text: 'Time Scale'
            },
            elements: {
                line: {
                    tension: 0 // disables bezier curves
                }
            },
            animation: {
                duration: 0 // general animation time
            },
            hover: {
                animationDuration: 0 // duration of animations when hovering an item
            },
            ticks: {
                sampleSize: 10
            },
            responsiveAnimationDuration: 0, // animation duration after a resize
            scales: {
                xAxes: {
                    type: 'time',
                    time: {
                        format: "DD MM YYYY hh",
                        unit: 'hour',
                        unitStepSize: 1,
                    },
                    distribution: 'series',
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
    }

    const ctx = canvas.getContext('2d')
    const myLine = new Chart(ctx, config)

    canvas.addEventListener("click", evt => {
        const test = myLine.getElementsAtEvent(evt);
        console.log(test)

        //config.data.datasets.forEach(dataset => {

        //    if (dataset._datasetIndex === test)
        //    dataset.data.forEach(function(dataObj) {
        //        dataObj.y = randomScalingFactor();
        //    });
        //});

        //window.myLine.update();
        // => activePoints is an array of points on the canvas that are at the same position as the click event.
    })
}
