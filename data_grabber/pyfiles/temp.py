import time
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
CHROMEDRIVER_PATH = '/usr/local/bin/chromedriver'
WINDOW_SIZE = "1920,1080"
chrome_options = Options()
chrome_options.add_argument("--headless")
chrome_options.add_argument("--window-size=%s" % WINDOW_SIZE)
chrome_options.add_argument('--no-sandbox')
driver = webdriver.Chrome(executable_path=CHROMEDRIVER_PATH,
                          options=chrome_options
                         )
driver.get("https://weather.com/weather/today/l/b565aa4d4c1111a09ad8c1ede054636671f50d38757a6527b35d96b987ef86a8")

str1 = "/dataStorage/" + str(time.time()) + ".png"

element = driver.find_element_by_xpath("/html/body/div[1]/main/div[2]/div[2]/div[1]/div/section/div")

element.screenshot(str1)

driver.close()
