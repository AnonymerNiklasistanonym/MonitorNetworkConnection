#!/usr/bin/env python3

from pandas import DataFrame, read_csv
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import pandas as pd
import numpy as np
from os import path

file = path.join('..', 'data', 'data.csv')
df = pd.read_csv(file)
print(df)

df.plot(kind='scatter', y='CONNECTION_SPEED_MS_AVG', x='DATE_ISO', color='red')
plt.show()
