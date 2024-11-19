import yfinance as yf
import csv

def get_stock_data(ticker, start_date, end_date):
    stock = yf.Ticker(ticker)
    # data = stock.history(period="1y", interval="1h").to_dict()
    data = stock.history(start=start_date, end=end_date, interval="1h").to_dict()

    ticker = stock.info['symbol']
    timestamps = list(data['Close'].keys())
    prices = data['Close']
    volumes = data['Volume']

    with open('stock_data.csv', mode='a') as file:
        writer = csv.writer(file)
        for timestamp in timestamps:
            writer.writerow([timestamp, ticker, prices[timestamp], volumes[timestamp]])

    print(f"Data for {ticker} has been saved to stock_data.csv")

if __name__ == "__main__":
    # Clear the file before writing the data
    with open('stock_data.csv', mode='w') as file:
        writer = csv.writer(file)
        writer.writerow(['timestamp', 'ticker', 'price', 'volume'])
    tickers = ['NVDA', 'AAPL', 'MSFT', 'AMZN', 'GOOGL', 'META', 'TSLA', 'TSM', 'AVGO', 'ORCL']
    for ticker in tickers:
        get_stock_data(ticker, '2024-01-01', '2024-11-18')

