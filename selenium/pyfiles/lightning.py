import time
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
CHROMEDRIVER_PATH = '/usr/local/bin/chromedriver'
WINDOW_SIZE = "1920,1080"
chrome_options = Options()
chrome_options.add_argument("--headless")
chrome_options.add_argument("--window-size=%s" % WINDOW_SIZE)
chrome_options.add_argument('--no-sandbox')
chrome_options.add_argument("--autoplay-policy=no-user-gesture-required")
driver = webdriver.Chrome(executable_path=CHROMEDRIVER_PATH,
                          options=chrome_options
                         )
driver.get("https://www.weather.gov/zse/SatRad?speed=0.4&mins=10&count=7&lt=33.7974&ln=-98.8898&zm=4&rad=1&ltg=1&vis=0&ir=0&fc=0&hover=0&size=0&")

time.sleep(15)

str1 = "/dataStorage/lightning" + str(time.time()) + ".png"

element = driver.find_element_by_xpath("/html/body/div[3]/div[2]/div[3]/div/div/div[3]/div/div/div/div[2]/canvas")

element.screenshot(str1)

driver.close()
