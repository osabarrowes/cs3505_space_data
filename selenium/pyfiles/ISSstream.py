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
driver.get("https://www.ustream.tv/channel/iss-hdev-payload/pop-out")

time.sleep(5)

str1 = "/dataStorage/Screenshot" + str(time.time()) + ".png"

driver.save_screenshot(str1)

driver.close()
