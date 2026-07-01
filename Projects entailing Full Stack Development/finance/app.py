import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Muliple SELECTs n stocks owned, no. of shares owned, current price, total value of holding, current cash

    rows = db.execute(
        "SELECT symbol, SUM(shares) AS total FROM transactions WHERE user_id = ? GROUP BY symbol HAVING total != 0", session["user_id"])
    for row in rows:
        quote = lookup(row["symbol"])
        row["price"] = quote["price"]
        row["value"] = quote["price"] * row["total"]

    rows1 = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = rows1[0]["cash"]
    total = sum(row["value"] for row in rows)
    grand_total = total + cash

    return render_template("index.html", rows=rows, total=total, grand_total=grand_total, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("could not return value")
        if not shares:
            return apology("could not return value")
        if not shares.isdigit() or int(shares) <= 0:
            return apology("not a positive integer")

        quote = lookup(symbol)
        if not quote:
            return apology("not a valid symbol")

        # Call lookup for current price

        shares = int(shares)
        cost = shares * quote["price"]
        rows = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"]
        )
        cash = rows[0]["cash"]

        if cash < cost:
            return apology("insufficient cash", 400)

        # Add tables for username, quote.price and time
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", cost, session["user_id"])
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol, shares, quote["price"])

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute(
        "SELECT * FROM transactions WHERE user_id = ? ORDER BY transacted DESC", session["user_id"])

    # User reached route via GET (as by clicking a link or via redirect)
    return render_template("history.html", history=history)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)
        else:
            symbol = request.form.get("symbol")
            quote = lookup(symbol)

        if quote is None:
            return apology("invalid symbol", 400)

        # User reached route via GET (as by clicking a link or via redirect)
        return render_template("quoted.html", quote=quote)
    else:

        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        name = request.form.get("username")
        password = request.form.get("password")
        confirmationPassword = request.form.get("confirmation")
        if not name:
            return apology("must provide username", 400)

        # Ensure password, confirmation password and name was submitted
        elif not password:
            return apology("must provide password", 400)

        elif not confirmationPassword:
            return apology("must provide confirmation password", 400)

        elif not password == confirmationPassword:
            return apology("must provide the same passwords", 400)
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username does not exist
        if len(rows) != 0:
            return apology("username is taken", 400)

        # INSERT new user, storing the hash (never the raw password)
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            name,
            generate_password_hash(password),
        )

        # Log them in automatically
        rows = db.execute("SELECT * FROM users WHERE username = ?", name)
        session["user_id"] = rows[0]["id"]

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
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)
        if not shares or not shares.isdigit() or int(shares) <= 0:
            return apology("not a positive integer")
        elif not lookup(symbol):
            return apology("not a valid symbol")

        shares = int(shares)
        quote = lookup(symbol)

        # Call lookup for current price

        cost = shares * quote["price"]
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]
        rows1 = db.execute(
            "SELECT SUM(shares) AS total FROM transactions WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        sharesHeld = rows1[0]["total"]

        if sharesHeld == None:
            return apology("No stocks held")
        if shares > sharesHeld:
            return apology("exceeded stocks")

        proceeds = shares * quote["price"]
        # Add tables for username, quote.price and time
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", proceeds,  session["user_id"])
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
                   session["user_id"], symbol, -shares, quote["price"])

        return redirect("/")

    else:
        rows = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
        return render_template("sell.html", rows=rows)


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Change password"""
    # User reached route via POST(as by submitting a form via POST)
    if request.method == "POST":
        newPassword = request.form.get("newPassword")
        oldPassword = request.form.get("oldPassword")

        if not newPassword or not oldPassword:
            return apology("empty text fields")

        if newPassword == oldPassword:
            return apology("Old password and new password can not be the same")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE id = ?",  session["user_id"])
        if not check_password_hash(
            rows[0]["hash"], request.form.get("oldPassword")
        ):
            return apology("invalid password", 403)

        db.execute("UPDATE users SET hash = ? WHERE id = ?",
                   generate_password_hash(newPassword),  session["user_id"])

        # Redirect user to home page
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("password.html")
