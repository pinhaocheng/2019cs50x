import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # get stocks and numbers owned
    stocks = db.execute(
        "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0", user_id=session["user_id"])
    quotes = {}
    total_val = {}

    # check for user's available cash
    user_info = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
    avail_cash = user_info[0]["cash"]

    grand_total = avail_cash

    # get current price
    for stock in stocks:
        quotes[stock["symbol"]] = lookup(stock["symbol"])
        total_val[stock["symbol"]] = quotes[stock["symbol"]]["price"] * stock["total_shares"]
        grand_total = grand_total + total_val[stock["symbol"]]

    return render_template("index.html", quotes=quotes, stocks=stocks, cash=avail_cash, total_val=total_val, grand_total=grand_total)


@app.route("/add-fund", methods=["GET", "POST"])
@login_required
def add_fund():
    """Add fund into cash account"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # check for integer
        # https://stackoverflow.com/questions/5424716/how-to-check-if-string-input-is-a-number
        try:
            money_in = int(request.form.get("fund"))
        except:
            return apology("amount invalid", 400)

            # update cash left
        db.execute("UPDATE users SET cash = cash + :transaction WHERE id = :user_id",
                   transaction=money_in, user_id=session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("add_fund.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # retrieve stock quote
        quote = lookup(request.form.get("symbol"))

        # check if symbol is valid
        if not quote:
            return apology("Stock Symbol invalid", 400)

        # check for integer
        # https://stackoverflow.com/questions/5424716/how-to-check-if-string-input-is-a-number
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("It's NUMBER of share, not TEXT", 400)

        # check for positive
        if not (shares > 0):
            return apology("Number of shares must be larger than 0", 400)

        # check for user's available cash
        user_info = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        avail_cash = user_info[0]["cash"]

        # check if cash is enough
        if ((quote["price"] * shares) > avail_cash):
            return apology("insufficient funds", 400)

        # update cash left
        db.execute("UPDATE users SET cash = cash - :transaction WHERE id = :user_id",
                   transaction=quote["price"]*shares, user_id=session["user_id"])

        # add stock into portfolio
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES(:user_id, :symbol, :shares, :price)",
                   user_id=session["user_id"],
                   symbol=request.form.get("symbol"),
                   shares=shares,
                   price=quote["price"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""

    # getting username from url
    username = request.args.get('username')

    # return false when username typed is found in database
    if (db.execute("SELECT username FROM users WHERE username = :username", username=username)):
        return jsonify(False)
    else:
        return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute(
        "SELECT symbol, shares, price, time FROM transactions WHERE user_id = :user_id ORDER BY time ASC", user_id=session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # retrieve stock quote
        quote = lookup(request.form.get("symbol"))

        if not quote:
            return apology("Stock Symbol invalid", 400)

        # format price into USD
        quote["price"] = usd(quote["price"])

        # display price
        return render_template("quoted.html", quote=quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation password was submitted
        elif not request.form.get("confirmation"):
            return apology("must re-type password for confirmation", 400)

        # making sure both password are the same
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("Password entered not the same", 400)

        # generate hash
        hash = generate_password_hash(request.form.get("password"))

        # insert new user info
        new_user = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                              username=request.form.get("username"), hash=hash)

        # prompt apology if username taken
        if not new_user:
            return apology("Username taken", 400)

        # Remember which user has logged in
        session["user_id"] = new_user

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # return apology('no problemo',888)
        # retrieve stock quote
        quote = lookup(request.form.get("symbol"))

        # get stocks and numbers owned
        stock = db.execute("SELECT SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id AND symbol = :symbol GROUP BY symbol",
                           user_id=session["user_id"], symbol=request.form.get("symbol"))

        # var for number of shares selling
        shares = int(request.form.get("shares"))

        # check user owned stocks is sufficient to sell
        if stock[0]["total_shares"] < shares:
            return apology("insufficient unit to sell", 400)

        # check for user's available cash
        user_info = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        avail_cash = user_info[0]["cash"]

        # update cash left
        db.execute("UPDATE users SET cash = cash + :transaction WHERE id = :user_id",
                   transaction=quote["price"]*shares, user_id=session["user_id"])

        # add stock into portfolio
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES(:user_id, :symbol, :shares, :price)",
                   user_id=session["user_id"],
                   symbol=request.form.get("symbol"),
                   shares=-shares,
                   price=quote["price"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # get stocks and numbers owned
        stocks = db.execute(
            "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0", user_id=session["user_id"])

        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
